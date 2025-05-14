#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Parachute.generated.h"

class APlayerCharacter;
class UPlayerMovementComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class PUBG_API AParachute : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AParachute();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Timeline for parachute animation
    UFUNCTION()
    void TimelineStart(APlayerCharacter* CachedPlayer);

    // Called during the timeline update to change parachute scale
    UFUNCTION()
    void OnTimelineProgress(float Value);
    
    // For replication of relevant variables
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
    // Player character reference
    UPROPERTY()
    APlayerCharacter* PlayerCharacter;

    // Player movement component reference
    UPROPERTY(Replicated)
    UPlayerMovementComponent* MovementComponent;

    // Timeline component for parachute animation
    UPROPERTY()
    UTimelineComponent* TimelineComponent;

    // Parachute curve
    UPROPERTY()
    UCurveFloat* ParachuteCurve;


};
