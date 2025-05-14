// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PlayMontageAndWaitForEvent.generated.h"

class UBaseAbilitySystemComponent;

/** Delegate type used, EventTag and Payload may be empty if it came from the montage callbacks */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

/**
 * This task combines PlayMontageAndWait and WaitForEvent into one task, so you can wait for multiple types of activations such as from a melee combo
 * Much of this code is copied from one of those two ability tasks
 * This is a good task to look at as an example when creating game-specific tasks
 * It is expected that each game will have a set of game-specific tasks to do what they want
 */
UCLASS()
class PUBG_API UPlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

	
public:
	// 생성자 및 재정의
	UPlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	/**
	* 이 작업의 블루프린트 노드인 PlayMontageAndWaitForEvent에는 자동으로 Activate()를 호출하는 플러그인의 흑마술이 있습니다.
	* AbilityTask.h에 명시된 대로 K2Node_LatentAbilityCall 내부. C++로 작성된 능력 로직은 아마도 Activate() 자체를 수동으로 호출해야 할 것입니다.
	*/
	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool AbilityEnded) override;

	/** 몽타주 재생이 완전히 끝났습니다 */
	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventDelegate OnCompleted;

	/** 몽타주가 블렌딩 아웃되기 시작했습니다 */
	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventDelegate OnBlendOut;

	/** 몽타주가 중단되었습니다 */
	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventDelegate OnInterrupted;

	/** 해당 능력 작업이 다른 능력에 의해 명시적으로 취소되었습니다 */
	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventDelegate OnCancelled;

	/** 촉발된 게임플레이 이벤트 중 하나가 발생했습니다 */
	UPROPERTY(BlueprintAssignable)
	FPlayMontageAndWaitForEventDelegate EventReceived;

     /**
	 * 몽타주를 재생하고 끝날 때까지 기다립니다. EventTags(또는 EventTags가 비어 있음)와 일치하는 게임플레이 이벤트가 발생하면 EventReceived 대리자는 태그 및 이벤트 데이터와 함께 실행됩니다.
	 * StopWhenAbilityEnds가 참인 경우, 능력이 정상적으로 종료되면 이 몽타주는 중단됩니다. 능력이 명시적으로 취소되면 항상 중지됩니다.
	 * 일반적인 실행에서는 몽타주가 블렌딩 아웃될 때 OnBlendOut이 호출되고, 재생이 완전히 끝나면 OnCompleted가 호출됩니다.
	 * 다른 몽타주가 이를 덮어쓰면 OnInterrupted가 호출되고, 능력이나 작업이 취소되면 OnCancelled가 호출됩니다.
	 *
	 * @param TaskInstanceName 나중에 쿼리하기 위해 이 작업의 이름을 재정의하도록 설정합니다.
	 * @param MontageToPlay 캐릭터에서 재생할 몽타주
	 * @param EventTags 이 태그와 일치하는 모든 게임플레이 이벤트는 EventReceived 콜백을 활성화합니다. 비어 있으면 모든 이벤트가 콜백을 트리거합니다.
	 * @param Rate 변경으로 몽타주를 더 빠르게 또는 느리게 재생
	 * @param bStopWhenAbilityEnds true인 경우, 능력이 정상적으로 종료되면 이 몽타주가 중단됩니다. 능력이 명시적으로 취소되면 항상 중지됩니다.
	 * @param AnimRootMotionTranslationScale 루트 모션의 크기를 수정하려면 변경하거나 완전히 차단하려면 0으로 설정하세요.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UPlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(
			UGameplayAbility* OwningAbility,
			FName TaskInstanceName,
			UAnimMontage* MontageToPlay,
			FGameplayTagContainer EventTags,
			float Rate = 1.f,
			FName StartSection = NAME_None,
			bool bStopWhenAbilityEnds = true,
			float AnimRootMotionTranslationScale = 1.f);

private:
	/** 재생 중인 몽타주 */
	UPROPERTY()
	UAnimMontage* MontageToPlay;

	/** 게임플레이 이벤트와 일치시킬 태그 목록 */
	UPROPERTY()
	FGameplayTagContainer EventTags;

	/** 재생 속도 */
	UPROPERTY()
	float Rate;

	/** 몽타주를 시작할 섹션 */
	UPROPERTY()
	FName StartSection;

	/** 루트 모션 움직임을 적용하는 방법을 수정합니다 */
	UPROPERTY()
	float AnimRootMotionTranslationScale;

	/** 능력이 끝나면 오히려 몽타주가 중단되어야 합니다 */
	UPROPERTY()
	bool bStopWhenAbilityEnds;

	/** 능력이 몽타주를 재생하고 있는지 확인하고 해당 몽타주를 중지합니다. 몽타주가 중지되면 참을 반환하고, 그렇지 않으면 거짓을 반환합니다. */
	bool StopPlayingMontage();

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);
	
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
	
};
