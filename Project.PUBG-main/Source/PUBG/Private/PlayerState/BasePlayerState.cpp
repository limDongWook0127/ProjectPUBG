// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BasePlayerState.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "Character/PlayerCharacter.h"
#include "Controller/BasePlayerController.h"
#include "GameState/BaseGameState.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/HUD/HudWidget.h"
#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"


ABasePlayerState::ABasePlayerState()
{
	bReplicates = true;
	
	// 능력 시스템 구성요소를 생성하고 명시적으로 복제되도록 설정합니다.
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// 혼합 모드는 GE가 시뮬레이션된 프록시에 복제되지 않고 GE만 자신에게 복제된다는 의미입니다. 다른 GDPlayerState(Hero)가 GE를 수신하면,
	// 서버에서는 이에 대해 알려주지 않습니다. 속성, GameplayTags 및 GameplayCue는 계속해서 복제됩니다.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	// 속성 세트를 생성합니다. 이는 기본적으로 복제됩니다.
	// AbilitySystemComponent를 소유한 액터의 하위 객체로 추가
	// AbilitySystemComponent에 AttributeSet을 자동으로 등록합니다.
	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));
	
	// PlayerState의 NetUpdateFrequency를 캐릭터와 동일하게 설정합니다.
	// PlayerStates의 기본값은 매우 낮으며 능력 시스템에 지연이 발생합니다.
	// 100은 출시 게임에 비해 너무 높을 수 있으므로 필요에 맞게 조정할 수 있습니다.
	NetUpdateFrequency = 100.0f;

	// Cache tags
	DeadTag = BaseGameplayTag::Player_State_Dead;
}

class UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

class UBaseAttributeSet* ABasePlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ABasePlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ABasePlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ABasePlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ABasePlayerState::GetHealthRegenRate() const
{
	return AttributeSetBase->GetHealthRegenRate();
}

float ABasePlayerState::GetStamina() const
{
	return AttributeSetBase->GetStamina();
}

float ABasePlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float ABasePlayerState::GetStaminaRegenRate() const
{
	return AttributeSetBase->GetStaminaRegenRate();
}

float ABasePlayerState::GetArmor() const
{
	return AttributeSetBase->GetArmor();
}

float ABasePlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

float ABasePlayerState::GetMagazine() const
{
	return AttributeSetBase->GetMagazine();
}

float ABasePlayerState::GetMaxMagazine() const
{
	return AttributeSetBase->GetMaxMagazine();
}

float ABasePlayerState::GetKillCount() const
{
	return AttributeSetBase->GetKillCount();
}

float ABasePlayerState::GetRank() const
{	
	return AttributeSetBase->GetRank();
}

void ABasePlayerState::SetRank(float Rank) const
{
	AttributeSetBase->SetRank(Rank);
}

bool ABasePlayerState::HasDeadTag() const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(DeadTag);
}

void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ABasePlayerState::HealthChanged);
	StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &ABasePlayerState::StaminaChanged);
	KillCountChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetKillCountAttribute()).AddUObject(this, &ABasePlayerState::KillCountChanged);
	RankChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetRankAttribute()).AddUObject(this, &ABasePlayerState::RankChanged);
}

void ABasePlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{	
	//UE_LOG(LogTemp, Warning, TEXT("HealthChanged"));
	float Health = Data.NewValue;

	ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	if (PC && PC->IsLocalController())
	{
		if(UHudWidget* Hud = PC->GetHudWidget())
        {
			Hud->GetPlayerStatusWidget()->SetHealth(Health);
        }		
	}
	
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
		if (Player)
		{			
			Player->Die();
			
			if (HasAuthority())
			{				
				ABaseGameState* GS = Cast<ABaseGameState>(GetWorld()->GetGameState());				

				AbilitySystemComponent->CancelAllAbilities();
				AbilitySystemComponent->ResetAbilitySystem();
				AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
				
				Player->SetRank(GS->CurrentPlayerCount);

				if (GS)
				{
					GS->UpdatePlayerCount();			
				}
			}
		}
	}	
}

void ABasePlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;
}

void ABasePlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegenRate = Data.NewValue;

}

void ABasePlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{	
	float Stamina = Data.NewValue;

	ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	if (PC && PC->IsLocalController())
	{
		if(UHudWidget* Hud = PC->GetHudWidget())
		{
			Hud->GetPlayerStatusWidget()->SetStamina(Stamina);
		}		
	}
}

void ABasePlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)  
{
	float MaxStamina = Data.NewValue;

	// Update floating status bar
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
}

void ABasePlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float StaminaRegenRate = Data.NewValue;
}

void ABasePlayerState::KillCountChanged(const FOnAttributeChangeData& Data)
{
	float KillCount = Data.NewValue;

	ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	if (PC && PC->IsLocalController())
	{
		PC->ShowKillLog(KillCount);
		PC->UpdateKillCount(KillCount);
	}
}

void ABasePlayerState::RankChanged(const FOnAttributeChangeData& Data)
{
	float Rank = Data.NewValue;

	ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	if (PC && PC->IsLocalController())
	{
		PC->ShowResultWidget(Rank);
	}	
}