// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "PUBGSpringArmComponent.generated.h"
	
class UTimelineComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UPUBGSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
public:
	UPUBGSpringArmComponent();
	FVector GetCurrentOffset() const;
	FVector SetCurrentOffset(FVector NewVector);
	void AddOffset(const FVector& OffsetDelta);
	void TimelineAddOffset(FVector& OffsetDelta, float Duration);
	UFUNCTION(BlueprintCallable)
	void BP_TimelineAddOffset(FVector OffsetDelta, float Duration);
	UFUNCTION()
	void OnTimelineUpdate(float Alpha);
	UFUNCTION()
	void OnTimelineFinished();

protected:
	// 카메라 오프셋을 저장하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector CurrentOffset;
	
	UPROPERTY()
	UCurveFloat* FloatCurve;
	
	UPROPERTY()
	UTimelineComponent* TimelineComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector TargettingOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector InitialOffset;
	
};

