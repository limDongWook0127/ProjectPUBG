// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"

// 무브먼트
#include "Component/Movement/PlayerMovementComponent.h"



// Sets default values
ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// 틱관련 기능 비활성화
	 PrimaryActorTick.bCanEverTick = true;
	 PrimaryActorTick.bStartWithTickEnabled = true;
	
	BaseAbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("BaseAbilitySystemComponent"));
	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ABaseCharacter::GetHealth() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxHealth() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetStamina() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetStamina();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxStamina() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float ABaseCharacter::GetMoveSpeed() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float ABaseCharacter::GetMagazine() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMagazine();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxMagazine() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMaxMagazine();
	}

	return 0.0f;
}

float ABaseCharacter::GetArmor() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetArmor();
	}

	return 0.0f;
}

float ABaseCharacter::GetKillCount() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetKillCount();
	}

	return 0.0f;
}


float ABaseCharacter::GetRank() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetRank();
	}

	return 0.0f;
}


UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return BaseAbilitySystemComponent.Get();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);	
}


void ABaseCharacter::ApplyHealthRegenEffect() const
{	
	if (BaseAbilitySystemComponent.Get() && GE_HealthRegen)
	{
		FGameplayEffectSpecHandle HealthRegenEffect = BaseAbilitySystemComponent->MakeOutgoingSpec(GE_HealthRegen, 1.0f, BaseAbilitySystemComponent->MakeEffectContext());
		if (HealthRegenEffect.IsValid())
		{			
			BaseAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*HealthRegenEffect.Data.Get());		
		}
	}
}

void ABaseCharacter::RemoveHealthRegenEffect() const
{
	if (BaseAbilitySystemComponent.Get() && GE_HealthRegen)
	{
		BaseAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GE_HealthRegen, BaseAbilitySystemComponent.Get());
	}
}

void ABaseCharacter::ApplyStaminaDecayEffect() const
{
	if (BaseAbilitySystemComponent.Get() && GE_StaminaDecay)
	{
		FGameplayEffectSpecHandle StaminaDecayEffect = BaseAbilitySystemComponent->MakeOutgoingSpec(GE_StaminaDecay, 1.0f, BaseAbilitySystemComponent->MakeEffectContext());
		if (StaminaDecayEffect.IsValid())
		{
			BaseAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StaminaDecayEffect.Data.Get());
		}
	}
}

void ABaseCharacter::RemoveStaminaDecayEffect() const
{
	if (BaseAbilitySystemComponent.Get() && GE_StaminaDecay)
	{
		BaseAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GE_StaminaDecay, BaseAbilitySystemComponent.Get());
	}
}

void ABaseCharacter::SetHealth(float Health)
{
	if (BaseAttributeSet.IsValid())
	 {
	     BaseAttributeSet->SetHealth(Health);
	 }
}

void ABaseCharacter::SetStamina(float Stamina)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetStamina(Stamina);
	}
}

void ABaseCharacter::SetArmor(float Armor)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetArmor(Armor);
	}
}

void ABaseCharacter::SetMoveSpeed(float MoveSpeed)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetMoveSpeed(MoveSpeed);
	}
}

void ABaseCharacter::SetMagazine(float Magazine)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetMagazine(Magazine);
	}
}

void ABaseCharacter::SetMaxMagazine(float MaxMagazine)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetMaxMagazine(MaxMagazine);
	}
}

void ABaseCharacter::SetKillCount(float KillCount)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetKillCount(KillCount);
	}
}

void ABaseCharacter::SetRank(float Rank)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetRank(Rank);
	}
}




