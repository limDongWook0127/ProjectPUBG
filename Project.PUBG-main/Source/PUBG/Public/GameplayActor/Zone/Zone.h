// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Zone.generated.h"

class UGameplayEffect;
class APlayerCharacter;

UCLASS()
class PUBG_API AZone : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AZone();

	UFUNCTION()
	void NotifySize();
	
	UFUNCTION()
	void StartShirnkZone();
	
	UFUNCTION()
	void UpdateShrinkZone(float Value);

	UFUNCTION()
	void TimelineFinishedFunction();

	//다음 랜덤 범위
	UFUNCTION()
	FVector GetRandomPointInCircle(FVector OriginCenter,float NextRadius);

	UFUNCTION()
	float GetCurrentRadius();

	UFUNCTION()
	float GetMeshWorldScale() const;

	UFUNCTION()
	void UpdateZoneDamage(APlayerCharacter* PlayerCharacter);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;	

	void UpdateCurrentZone();
	void UpdateNextZone();
	void StartShrinkTimer();
	void SetShirnkTime(int NewShirnkTime);

private:		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloat;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ZoneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ShirnkTime;

	float NextScale;
	float CurrentScale;
	
	FVector CurrentLocation;
	FVector NextLocation;
	
	bool bIsVisibiltyNextZone;		
	float ShrinkFactor;

};



