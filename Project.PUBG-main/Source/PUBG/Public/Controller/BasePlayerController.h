// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UResultWidget;
class UInputMappingContext;
class UWorldMapWidget;
class AAirplane;
class UHudWidget;
class UInventoryWidget;
class UNotificationWidget;
class ACrosshairHUD;

/**
 * 
 */
UCLASS()
class PUBG_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:
	ABasePlayerController();
	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlayingState() override;
	
private:
	

public:	
	UFUNCTION(BlueprintCallable)
	void CreateHUD();
	UFUNCTION(BlueprintCallable)
	void InputModeUI();
	UFUNCTION(BlueprintCallable)
	void InputModeGame();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void HitEventServer(AActor* TargetActor, FGameplayTag HitTag, FGameplayEventData payload);
	
	
	//Getter
	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION(BlueprintCallable)
	void CreateInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void ToggleMapWidget();
	UFUNCTION(Client, Reliable)
	void ClientCreateInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void DestroyInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void UpdateCurrentPlayer(int32 CurrentPlayer);
	UFUNCTION(BlueprintCallable)
	void UpdateKillCount(int32 KillCount);
	UFUNCTION(BlueprintCallable)
	void UpdateMapCurrentZone();
	UFUNCTION(BlueprintCallable)
	void UpdateMapNextZone();
	UFUNCTION(BlueprintCallable)
	void UpdateAirplanebVisibilty();
	UFUNCTION(BlueprintCallable)
	void UpdateAirplanebLocation();
	UFUNCTION(BlueprintCallable)
	void UpdateRedZone();
	UFUNCTION(BlueprintCallable)
	void ShowNotification(FText Text);
	UFUNCTION(BlueprintCallable)
	void ShowKillLog(int32 Killcount);
	UFUNCTION(BlueprintCallable)
	void ShowResultWidget(int32 Rank);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_AddMappingContext(AAirplane* NewControlledAirplane, UInputMappingContext* InputMappingContext);
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_RemoveMappingContext();
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_UpdateAirplanebFall();

	UFUNCTION(BlueprintCallable)
	UHudWidget* GetHudWidget() { return HudWidget; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UHudWidget> HudWidgetClass;
	UPROPERTY()
	UHudWidget* HudWidget;
		
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UNotificationWidget> NotificationWidgetClass;
	UPROPERTY()
	UNotificationWidget* NotificationWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UResultWidget> ResultWidgetClass;
	UPROPERTY()
	UResultWidget* ResultWidget;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	AAirplane* ControlledAirplane;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInputMappingContext* AdditionalInputMappingContext;
};

