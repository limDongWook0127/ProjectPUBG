#include "GameplayActor/Airplane/GA/GA_Airplane_Fall.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "Controller/BasePlayerController.h"
#include "GameInstance/BaseGameInstance.h"
#include "GameplayActor/Airplane/Airplane.h"
#include "GameplayActor/Parachute/Parachute.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/AirplaneManager.h"

UGA_Airplane_Fall::UGA_Airplane_Fall()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Airplane_Fall::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, 
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 타이머를 설정하기 전에 GetWorld()가 유효한지 확인
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is invalid!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	

	// 게임 인스턴스 가져오기
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	// 비행기 객체 가져오기
	AAirplane* Airplane = GI->GetAirplaneManager()->SpawnedPlane;
	if (!Airplane)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);	
		return;
	}

	// 플레이어 캐릭터와 컨트롤러 설정
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	ABasePlayerController* PlayerController = GetPlayerControllerFromActorInfo();

	// 비행기 위치로 플레이어 이동
	PlayerCharacter->SetActorLocation(Airplane->GetActorLocation());
	PlayerCharacter->SetActorRotation(Airplane->GetActorRotation());

	// 뷰 타겟을 플레이어 캐릭터로 설정
	PlayerController->SetViewTargetWithBlend(PlayerCharacter, 1.0f);
	PlayerController->Client_RemoveMappingContext();	
	PlayerController->Client_UpdateAirplanebFall();

	// 플레이어의 능력 비활성화
	for (TSubclassOf<UPlayerGameplayAbility> GAClass : PlayerGameplayAbilities)
	{
		GetBaseAbilitySystemComponentFromActorInfo()->RemoveAbilityByClass(GAClass);
	}

	// 플레이어를 자유 낙하 상태로 설정
	PlayerCharacter->Server_SetInFreefall(true);
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
	MovementComponent->GravityScale = 0.2f;
	EndAbility(Handle, ActorInfo, ActivationInfo, false, true);	
}


