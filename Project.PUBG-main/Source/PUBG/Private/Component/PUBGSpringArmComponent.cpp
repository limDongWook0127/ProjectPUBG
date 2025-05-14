// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PUBGSpringArmComponent.h"

#include "AnimNodes/AnimNode_RandomPlayer.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Actor.h"

UPUBGSpringArmComponent::UPUBGSpringArmComponent()
{
	// 기본 스프링 암 컴포넌트 초기화
	PrimaryComponentTick.bCanEverTick = true;
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>("TimelineComponent");
	if (CurrentOffset.IsZero())
	{
		InitialOffset = FVector(0.f, 55.f, 65.f);
		CurrentOffset = FVector(0.f, 55.f, 65.f);
		TargettingOffset = FVector(0.f, 55.f, 65.f);
	}
}

FVector UPUBGSpringArmComponent::GetCurrentOffset() const
{
	return CurrentOffset;
}

FVector UPUBGSpringArmComponent::SetCurrentOffset(FVector NewVector)
{
	return CurrentOffset = NewVector;
}

void UPUBGSpringArmComponent::AddOffset(const FVector& OffsetDelta)
{
	// 기존 오프셋에 Delta 값을 더하여 새로운 오프셋을 계산
	CurrentOffset += OffsetDelta;

	// 새로운 오프셋을 적용 (스프링 암의 상대 위치 업데이트)
	SocketOffset = CurrentOffset;
}

#pragma region TimelineComponent
void UPUBGSpringArmComponent::TimelineAddOffset(FVector& OffsetDelta, float Duration)
{
	TargettingOffset += OffsetDelta;

	// 로컬 좌표를 소켓의 오프셋으로 사용
	CurrentOffset = SocketOffset;


	// 타임라인에 사용할 곡선 생성
	FloatCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass());
	FloatCurve->FloatCurve.AddKey(0.0f, 0.0f); // 시작 점 (Alpha = 0)
	FloatCurve->FloatCurve.AddKey(Duration, 1.0f); // 끝 점 (Alpha = 1)

	if (TimelineComponent)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindDynamic(this, &UPUBGSpringArmComponent::OnTimelineUpdate);
		TimelineComponent->AddInterpFloat(FloatCurve, TimelineCallback);

		// 타임라인 완료 후 호출될 함수 설정
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindDynamic(this, &UPUBGSpringArmComponent::OnTimelineFinished);
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedCallback);
		TimelineComponent->PlayFromStart();
	}
}


void UPUBGSpringArmComponent::OnTimelineUpdate(float Alpha) //타임라인중 계속 실행되어야 할 함수
{
	FVector NewOffset = FMath::Lerp(CurrentOffset, TargettingOffset, Alpha);
	SocketOffset = NewOffset;
}


void UPUBGSpringArmComponent::OnTimelineFinished() // 타임라인 완료 후 호출되는 함수
{
}

void UPUBGSpringArmComponent::BP_TimelineAddOffset(FVector OffsetDelta, float Duration)
{
	TargettingOffset += OffsetDelta;

	// 로컬 좌표를 소켓의 오프셋으로 사용
	CurrentOffset = SocketOffset;


	// 타임라인에 사용할 곡선 생성
	FloatCurve = NewObject<UCurveFloat>(this, UCurveFloat::StaticClass());
	FloatCurve->FloatCurve.AddKey(0.0f, 0.0f); // 시작 점 (Alpha = 0)
	FloatCurve->FloatCurve.AddKey(Duration, 1.0f); // 끝 점 (Alpha = 1)

	if (TimelineComponent)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindDynamic(this, &UPUBGSpringArmComponent::OnTimelineUpdate);
		TimelineComponent->AddInterpFloat(FloatCurve, TimelineCallback);

		// 타임라인 완료 후 호출될 함수 설정
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindDynamic(this, &UPUBGSpringArmComponent::OnTimelineFinished);
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedCallback);
		TimelineComponent->PlayFromStart();
	}
}

#pragma endregion
