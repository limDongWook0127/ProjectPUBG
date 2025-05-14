// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayEffect.h"
#include "ApplyDamageToActor.generated.h"

/** Delegate type used, EventTag and Payload may be empty if it came from the montage callbacks */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FApplyDamageToActorDelegate);

/**
 * 
 */
UCLASS()
class PUBG_API UApplyDamageToActor : public UAbilityTask
{
	GENERATED_BODY()
public:
	/**
	 * 데미지 적용 작업을 생성하는 정적 함수
	 * @param OwningAbility - 작업을 소유한 Ability
	 * @param TaskInstanceName - 작업의 이름
	 * @param SourceActor - 데미지를 발생시키는 액터
	 * @param TargetActor - 데미지를 받을 액터
	 * @param DamageGameplayEffect - 데미지를 정의하는 GameplayEffect 클래스
	 * @param Damage - 적용할 데미지 값
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Apply Damage to Actor", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UApplyDamageToActor* ApplyDamageToActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* SourceActor, AActor* TargetActor, TSubclassOf<UGameplayEffect> DamageGameplayEffect, float Damage);

protected:
	/** 작업 실행 */
	virtual void Activate() override;

private:
	/** 데미지 적용 작업에 사용될 변수들 */
	AActor* SourceActor;
	AActor* TargetActor;
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;
	float Damage;

public:
	/** 작업 완료 이벤트 (블루프린트에서 바인딩 가능) */
	UPROPERTY(BlueprintAssignable)
	FApplyDamageToActorDelegate OnCompleted;
};
