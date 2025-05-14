// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "Character/PlayerCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Controller/BasePlayerController.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"

UBaseAttributeSet::UBaseAttributeSet()
{
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 이는 속성이 변경될 때마다 호출되므로 최대 체력의 경우 현재 합계를 일치하도록 조정하려고 합니다.
	Super::PreAttributeChange(Attribute, NewValue);

	// 최대값이 변경되면 전류를 조정하여 전류의 현재 %를 최대값으로 유지합니다.
	if (Attribute == GetMaxHealthAttribute()) // GetMaxHealthAttribute는 헤더 상단에 정의된 매크로에서 나옵니다.
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxMagazineAttribute()) // GetMaxMagazineAttribute는 헤더 상단에 정의된 매크로에서 나옵니다.
	{
		//AdjustAttributeForMaxChange(Magazine, MaxMagazine, NewValue, GetMagazineAttribute());
	}	
	else if (Attribute == GetMoveSpeedAttribute())
	{
		// 0단위/초 미만으로 둔화할 수 없으며 500단위/초 이상 증폭할 수 없습니다.
		NewValue = FMath::Clamp<float>(NewValue, 0, 500);
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	//우리의 소유자가 되어야 하는 Target 액터를 가져옵니다.
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	APlayerCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<APlayerCharacter>(TargetActor);
	}

	// 소스 액터 가져오기
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	APlayerCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{		
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<APlayerCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<APlayerCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		
		// 적중 결과를 추출해 봅니다.
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// 피해량의 로컬 복사본을 저장하고 피해 속성을 지웁니다.
		const float LocalDamageDone = GetDamage();
		
		SetDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// 피해가 추가되기 전에 캐릭터가 살아 있었다면 피해를 처리합니다.
			// 죽은 물체에 피해가 추가되거나 사망 애니메이션이 재생되는 것을 방지합니다.
			bool WasAlive = true;

			if (TargetCharacter)
			{				
				WasAlive = TargetCharacter->IsAlive();
			}
		
			// 상태 변경을 적용한 다음 고정합니다.
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			if (!TargetCharacter->IsAlive())
			{
				ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));				
				SourceCharacter->SetKillCount(SourceCharacter->GetKillCount() + 1.0f);
				//UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), TEXT(__FUNCTION__), *TargetCharacter->GetName());
			}
			
			if (TargetCharacter && WasAlive)
			{				
				// 받은 피해에 대한 로그 문입니다. 라이브 게임에서는 사용 중지되었습니다.
				//UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

				// 멀티캐스트 RPC로 HitReact 애니메이션과 사운드를 재생합니다.
				//const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();

				//if (Hit)
				//{
					// 기본값은 전면입니다.
					// 맞은 방향에 따라 다른 모션을 하고 싶다면 여기서 분기하면 됩니다
					//TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				//}				
			}
		}
	}// Damage
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 기타 상태 변경을 처리합니다.
		// 체력 손실은 피해를 거쳐야 합니다.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	} // Health
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		// 체력 변화를 처리합니다.
		float NewStamina = FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina());
		
		if (NewStamina > 90.0f)
		{
			SetHealthRegenRate(4.f);			
		}
		else if (NewStamina > 60.0f)
		{
			SetHealthRegenRate(3.0f);	
			SetMoveSpeed(350.0f * 1.0625f);		
		}
		else if (NewStamina > 20.0f)
		{
			SetHealthRegenRate(2.0f);
			SetMoveSpeed(350.0f * 1.025f);			
		}
		else if (NewStamina > 0.0f)
		{
			SetHealthRegenRate(1.0f);
			SetMoveSpeed(350.0f * 1.01f);
		}
		else
		{			
			SetHealthRegenRate(0.0f);
			SetMoveSpeed(350.0f);
		}
		
		if (NewStamina > 0.0f)
		{
			TargetCharacter->ApplyHealthRegenEffect();
			TargetCharacter->ApplyStaminaDecayEffect();
		}
		else
		{
			TargetCharacter->RemoveHealthRegenEffect();
			TargetCharacter->RemoveStaminaDecayEffect();
		}
		
		SetStamina(NewStamina);
	}
	else if (Data.EvaluatedData.Attribute == GetMagazineAttribute())
	{
		// 장전된 총알 수 변경
		SetMagazine(FMath::Clamp(GetMagazine(), 0.0f, GetMaxMagazine()));
	}
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Magazine, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMagazine, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, KillCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Rank, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// 현재 Val / Max 백분율을 유지하기위해 현재 값을 변경.
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}	
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegenRate, OldHealthRegenRate);
}

void UBaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldStamina);
}

void UBaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldMaxStamina);
}

void UBaseAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, StaminaRegenRate, OldStaminaRegenRate);
}

void UBaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Armor, OldArmor);
}

void UBaseAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UBaseAttributeSet::OnRep_Magazine(const FGameplayAttributeData& OldMagazine)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Magazine, OldMagazine);
}

void UBaseAttributeSet::OnRep_MaxMagazine(const FGameplayAttributeData& OldMaxMagazine)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMagazine, OldMaxMagazine);
}

void UBaseAttributeSet::OnRep_KillCount(const FGameplayAttributeData& OldKillCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, KillCount, OldKillCount);
}

void UBaseAttributeSet::OnRep_Rank(const FGameplayAttributeData& OldRank)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Rank, OldRank);
}
