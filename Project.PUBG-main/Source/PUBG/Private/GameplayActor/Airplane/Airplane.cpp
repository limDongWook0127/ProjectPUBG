// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/Airplane/Airplane.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/TimelineComponent.h"

#include "GameState/BaseGameState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/BasePlayerState.h"

#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Controller/BasePlayerController.h"
#include "GameInstance/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/TimeManager.h"


#include "Net/UnrealNetwork.h"

// Sets default values
AAirplane::AAirplane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메쉬 생성
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
	RootComponent = PlaneMesh;

	// 스프링 암 생성 및 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(PlaneMesh);
	SpringArm->TargetArmLength = 800.0f;  // 카메라 거리 조절
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// 카메라 생성 및 부착
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	TimelineComponent =CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

	bReplicates = true;
}

void AAirplane::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAirplane, ServerLocation);
}

void AAirplane::SetTimelineComponent()
{
	// 타임라인에 사용할 곡선 생성
	UCurveFloat* MoveCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass());
	MoveCurve->FloatCurve.AddKey(0.0f, 0.0f); // 시작 점 (Alpha = 0)
	MoveCurve->FloatCurve.AddKey(Duration, 1.0f); // 끝 점 (Alpha = 1)
	
	// 타임라인 업데이트 함수 바인딩
	if (MoveCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindDynamic(this,&AAirplane::UpdatePlaneLocation);
		TimelineComponent->AddInterpFloat(MoveCurve, ProgressFunction);

		// 타임라인 종료 시 호출할 함수 바인딩
		FOnTimelineEvent FinishFunction;
		FinishFunction.BindDynamic(this,&AAirplane::OnPlaneMoveFinished);
		TimelineComponent->SetTimelineFinishedFunc(FinishFunction);
	}
}

// Called when the game starts or when spawned
void AAirplane::BeginPlay()
{
	Super::BeginPlay();

}

void AAirplane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())  // 클라이언트에서만 실행
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ServerLocation, DeltaTime, 5.0f); // Interp Speed: 15
		SetActorLocation(NewLocation);
	}
}

void AAirplane::OnRep_ServerLocation()
{	
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_ServerLocation: %s"), *ServerLocation.ToString());
}

void AAirplane::MovePlane(FVector Start, FVector End)
{
	SetTimelineComponent();
	
	StartLocation = Start;
	EndLocation = End;

	SetActorLocation(StartLocation);
	SetActorRotation((End - Start).GetSafeNormal().Rotation());
	ServerLocation = StartLocation;
	
	UpdateIsVisibiltyAirplane(true);

	SetViewTargetToPlane();

	// 타임라인 초기화
	TimelineComponent->PlayFromStart();
}

void AAirplane::UpdatePlaneLocation(float Alpha)
{
	if (HasAuthority())
	{		
		FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
		//SetActorLocation(NewLocation);
		RootComponent->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		ServerLocation = NewLocation;
		UpdateCurrentLocation();
	}	
}

void AAirplane::OnPlaneMoveFinished()
{	
	UE_LOG(LogTemp, Warning, TEXT("비행기 이동 완료!"));
	UpdateIsVisibiltyAirplane(false);

	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI)
	{
		GI->GetTimeManager()->NotifyClientsToArrivePlane();
	}
	
	Destroy();
}

void AAirplane::UpdateCurrentLocation()
{
	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
		if (GameState && GameState->HasAuthority())
		{
			GameState->UpdateCurrentAirplaneLocation(Location); 
		}
	}
}

void AAirplane::UpdateIsVisibiltyAirplane(bool bIsVisibiltyAirplane)
{
	if (HasAuthority())
	{
		ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
		if (GameState && GameState->HasAuthority())
		{
			GameState->UpdateIsVisibiltyAirplane(StartLocation, EndLocation, bIsVisibiltyAirplane); 
		}
	}
}

void AAirplane::Multicast_SetControlledAirplane_Implementation(ABasePlayerController* PC)
{
	if (PC)
	{
		PC->Client_AddMappingContext(this, this->AirplaneData.WeaponInputMappingContext);
	}
}

void AAirplane::GrantPlayerAirplaneAbilites(APlayerState* _PS)
{
	
	ABasePlayerState* PS = Cast<ABasePlayerState>(_PS);
	UBaseAbilitySystemComponent* ASC =  Cast<UBaseAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	if (ASC)
	{
		ABasePlayerController* PC = PS->GetOwner<ABasePlayerController>();
		if (PC)
		{
			//Multicast_SetControlledAirplane_Implementation(PC);
			PC->Client_AddMappingContext(this, this->AirplaneData.WeaponInputMappingContext);
		}
		
		// 능력 부여
		TArray<FGameplayAbilitySpecHandle> OutGrantedAbilitySpecHandles;
		ASC->GrantPlayerWeaponAbilities(this->AirplaneData.AirplaneAbilities, 1, OutGrantedAbilitySpecHandles);
		this->AssignGrantedAbilitySpecHandles(OutGrantedAbilitySpecHandles);
	}
}

void AAirplane::SetViewTargetToPlane()
{
	// 생성한 비행기에 사람 탑승 시키기
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	
	if (!GS)
	{
		return;
	}

	for (APlayerState* PS : GS->PlayerArray)
	{
		if (PS)
		{
			APlayerController* PC = PS->GetOwner<APlayerController>();
			if (PC)
			{
				PC->SetViewTargetWithBlend(this, 1.0f);
			}			
			GrantPlayerAirplaneAbilites(PS);
		}
	}
}

void AAirplane::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
{
	GrantedAbilitySpecHandles = SpecHandles;
}

TArray<FGameplayAbilitySpecHandle>& AAirplane::GetGrantedAbilitySpecHandles()
{
	return GrantedAbilitySpecHandles;
}

void AAirplane::SetDuration(float NewDuration)
{
	Duration = NewDuration;
}
