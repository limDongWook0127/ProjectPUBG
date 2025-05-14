// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DamageExecCalculation.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "BaseLibrary/BaseDebugHelper.h"

//캡처할 속성을 선언하고 소스 및 대상에서 캡처할 방법을 정의합니다.
struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FDamageStatics()
	{
		// GESpec 생성 시 스냅샷이 발생합니다.

		// 이 예에서는 소스에서 아무 것도 캡처하지 않지만 원하는 AttackPower 속성이 있을 수 있습니다.

		// ExecutionCalculation 아래의 CalculationModifier로 피해 GE에 설정된 선택적 피해를 캡처합니다.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Damage, Source, true);

		// 대상의 방어구를 캡처합니다. 스냅샷을 찍지 마세요.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DStatics;
	return DStatics;
}

UDamageExecCalculation::UDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;


	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// 어떤 버프를 사용해야 하는지에 영향을 미칠 수 있으므로 소스와 대상에서 태그를 수집합니다.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float Damage = 0.0f;
	// ExecutionCalculation 아래의 CalculationModifier로 피해 GE에 설정된 선택적 피해 값을 캡처합니다.
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// SetByCaller 손상이 존재하는 경우 추가합니다.
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // 여기에 데미지 부스터를 늘릴 수 있습니다.
	
	float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));
	
	if (MitigatedDamage > 0.f)
	{
		// 대상의 피해 메타 속성을 설정합니다.
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
	
	// Target ASC에 피해를 방송합니다.
	UBaseAbilitySystemComponent* TargetASC = Cast<UBaseAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UBaseAbilitySystemComponent* SourceASC = Cast<UBaseAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}
}
