#include "GameplayActor/Parachute/Parachute.h"
#include "Character/PlayerCharacter.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AParachute::AParachute()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	SetReplicates(true);
	// bReplicates = true;
	
}

// Called when the game starts or when spawned
void AParachute::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐릭터가 설정되지 않았다면 설정하기
	if (!PlayerCharacter)
	{
		if (HasAuthority())  // 서버에서만 실행
		{
			// 서버에서는 모든 플레이어에 대해 반복하여 처리
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				APlayerController* PlayerController = Iterator->Get();
                if (PlayerController)
                {
                    // 각 플레이어의 캐릭터를 가져오기
                    PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
                    if (PlayerCharacter)
                    {
                        MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
                        if (!MovementComponent)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("MovementComponent Not Exist"));
                        }
                       // SetOwner(PlayerCharacter);  // 플레이어 캐릭터를 소유자로 설정
                    	UE_LOG(LogTemp, Warning, TEXT("OWNER:%s"), *GetOwner()->GetName());
                    }
                }
            }
        }
        else  // 클라이언트에서만 실행
        {
            // 클라이언트에서는 자신만의 플레이어 컨트롤러로부터 캐릭터를 가져옵니다.
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            if (PlayerController)
            {
                PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
                if (PlayerCharacter)
                {
                    MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
                    if (!MovementComponent)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("MovementComponent Not Exist"));
                    }
                }
            }
        }
    }
}

// Called every frame
void AParachute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerCharacter)
	{ 
		if (PlayerCharacter->GetActorLocation().Z <= 5000.f && PlayerCharacter->GetInFreefall()&&PlayerCharacter->GetFirstAirplane())
		{
			PlayerCharacter->Server_SetInFreefall(false);
			PlayerCharacter->SetOntheParachute(true);
			UE_LOG(LogTemp, Warning, TEXT("123PlayerCharacter->GetOntheParachute(): %hhd"),PlayerCharacter->GetOntheParachute());
			//Server_ChangeVelocity();
			UE_LOG(LogTemp, Warning, TEXT("MovementComponent->VELOCITY : %s"), *MovementComponent->Velocity.ToString());
			TimelineStart(PlayerCharacter);
		}
	}
}

void AParachute::TimelineStart(APlayerCharacter* CachedPlayer)
{
	SetActorHiddenInGame(false);
	UE_LOG(LogTemp, Warning, TEXT("ParachuteStart"))
	if (TimelineComponent)
	{
		TimelineComponent = NewObject<UTimelineComponent>(this, FName("ParachuteTimeline"));
		TimelineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		TimelineComponent->SetNetAddressable();
		TimelineComponent->SetLooping(false);
		ParachuteCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass());
		ParachuteCurve->FloatCurve.AddKey(0.0f, 0.0f); // 시작 점 (Alpha = 0)
		ParachuteCurve->FloatCurve.AddKey(0.3f, 1.0f);
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindDynamic(this, &AParachute::OnTimelineProgress);
		TimelineComponent->AddInterpFloat(ParachuteCurve, TimelineCallback);
		TimelineComponent->RegisterComponent();
		TimelineComponent->Play();
	}
}

void AParachute::OnTimelineProgress(float Value)
{
	FVector NewScale = FMath::Lerp(FVector(0.f), FVector(1.f), Value);
	SetActorScale3D(NewScale);
}

void AParachute::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParachute, MovementComponent);
	
}

// 서버에서 속도 변경을 요청하는 함수
// void AParachute::Server_ChangeVelocity_Implementation()
// {
// 	// 서버에서 속도 변경
// 	if (HasAuthority())
// 	{
// 		PlayerVelocity = FVector(PlayerCharacter->GetVelocity().X, PlayerCharacter->GetVelocity().Y, ZSpeed);
// 		MovementComponent->Velocity = PlayerVelocity;
// 		
// 		// 모든 클라이언트에 속도 변경을 전파
// 		Multicast_ChangeVelocity(PlayerVelocity);
// 	}
// }
//
// bool AParachute::Server_ChangeVelocity_Validate()
// {
// 	return true;
// }
//
// // 멀티캐스트 함수, 모든 클라이언트에서 실행
// void AParachute::Multicast_ChangeVelocity_Implementation(FVector NewVelocity)
// {
// 	if (!HasAuthority()) // 서버가 아닌 클라이언트에서만 실행
// 	{
// 		// 클라이언트에서 속도 업데이트
// 		MovementComponent->Velocity = NewVelocity;
// 	}
// }
