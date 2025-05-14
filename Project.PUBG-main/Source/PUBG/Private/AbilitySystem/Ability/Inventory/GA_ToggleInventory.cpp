// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Inventory/GA_ToggleInventory.h"

#include "AbilitySystemComponent.h"
#include "Component/NearArea/NearComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "Controller/BasePlayerController.h"

UGA_ToggleInventory::UGA_ToggleInventory()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
	AbilityTags = FGameplayTagContainer(BaseGameplayTag::InputTag_Toggle_Inventory);
}

void UGA_ToggleInventory::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{	
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// if (!GetPlayerControllerFromActorInfo()->IsLocalPlayerController())
	// {
	// 	return;
	// }
	
	// 1. 인벤토리 위젯 생성
	if (GetPlayerControllerFromActorInfo()->GetInventoryWidget() == nullptr)
	{
		GetPlayerControllerFromActorInfo()->ClientCreateInventoryWidget();
		UInventoryWidget* InventoryWidget = GetPlayerControllerFromActorInfo()->GetInventoryWidget();
		//InventoryWidget->UpdateInventoryWidget();

		GetPlayerCharacterFromActorInfo()->GetDetectionItem()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetPlayerControllerFromActorInfo()->InputModeUI();
		
		APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
		//PlayerCharacter->GetInventoryComponent()->ServerUpdateInventory();
		InventoryWidget->UpdateInventoryWidget();
		InventoryWidget->UpdateNearItemSlotWidget();
		InventoryWidget->UpdateEquippedWidget();
		
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is NULL"));
			return;
		}

		if (!PlayerCharacter->GetDetectionItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("DetectionItem is NULL"));
			return;
		}
		
		// GetPlayerCharacterFromActorInfo()->GetDetectionItem()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// GetPlayerControllerFromActorInfo()->InputModeUI();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NoCollision"));
		GetPlayerCharacterFromActorInfo()->GetDetectionItem()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetPlayerControllerFromActorInfo()->DestroyInventoryWidget();
		//GetPlayerCharacterFromActorInfo()->GetNearComponent()->GetGroundItems().Empty();
		GetPlayerCharacterFromActorInfo()->GetNearComponent()->ServerEmptyGroundItem();
		GetPlayerControllerFromActorInfo()->InputModeGame();
	}
	

	
	//EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	
    // UInventoryWidget* InventoryWidget = GetPlayerControllerFromActorInfo()->GetInventoryWidget();
    //
    // if (InventoryWidget != nullptr)
    // {
    // 	InventoryWidget->UpdateInventoryWidget();
    //
    // 	if (TickTask == nullptr)
    // 	{
    // 		TickTask = UFindFarmingItem::ExecuteTaskTick(this);
    // 		TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
    // 		TickTask->OnAbilityTaskTick.AddDynamic(this, &ThisClass::FindFarmingItem);
    // 		TickTask->SetWaitingOnAvatar();  // Ability와 Task의 수명 연동
    // 	}
    // 	
    // 	if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
    // 	{
    // 		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    // 		GetPlayerControllerFromActorInfo()->InputModeUI();
    // 		
    //
    // 		// 새로운 TickTask 생성
    // 		if (TickTask)
    // 		{
    // 			UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick1"));
    // 			TickTask->ReadyForActivation();
    // 		}
	   //
    // 		//EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
    // 		//CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
    // 	}
    	
    	// if (TickTask == nullptr)
    	// {
	    //     TickTask = UFindFarmingItem::ExecuteTaskTick(this);
    	// 	TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
    	// 	TickTask->OnAbilityTaskTick.AddDynamic(this, &ThisClass::FindFarmingItem);
    	// 	TickTask->SetWaitingOnAvatar();  // Ability와 Task의 수명 연동
    	// }
        
        // 인벤토리가 켜져있으면 끄기
        // if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
        // {
        //     InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
        //     GetPlayerCharacterFromActorInfo()->InputModeGame();
        //
        //     // 기존 TickTask 종료 처리
        //     if (TickTask)
        //     {
        //     	UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick"));
        //     	TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
        //         TickTask->EndTask();
        //         TickTask = nullptr;  // null로 초기화
        //     }
        //
        //     EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        // }
        // // 인벤토리가 꺼져있으면 켜기
        // else if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
        // {
        //     InventoryWidget->SetVisibility(ESlateVisibility::Visible);
        //     GetPlayerCharacterFromActorInfo()->InputModeUI();
        //     
        //     // 새로운 TickTask 생성
        //     if (TickTask)
        //     {
        //     	UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick1"));
        //         TickTask->ReadyForActivation();
        //     }
        // 	
        // 	//EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        // 	//CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
        //
        // }
    
}


void UGA_ToggleInventory::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("EndAbility"));


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	// UInventoryWidget* InventoryWidget = GetPlayerControllerFromActorInfo()->GetInventoryWidget();
	// if (InventoryWidget != nullptr)
	// {
	// 	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	// 	{
	// 		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	// 		GetPlayerControllerFromActorInfo()->InputModeGame();
	// 		// 기존 TickTask 종료 처리
	// 		if (TickTask)
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick"));
	// 			TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
	// 			TickTask->EndTask();
	// 			TickTask = nullptr;  // null로 초기화
	// 		}
	// 		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	// 	}
	// }
	// Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_ToggleInventory::FindFarmingItem(float DeltaTime)
{

	//EObjectTypeQuery TraceType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1);
	//BoxTraceChannel.Add(TraceType);
	
	EObjectTypeQuery TraceType1 = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2);
	BoxTraceChannel.Add(TraceType1);
	
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		//GetPlayerCharacterFromActorInfo()->GetActorLocation()+GetPlayerCharacterFromActorInfo()->GetActorForwardVector()* TraceDistance,
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		TraceBoxSize/ 2,
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		//bIsDebugging?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
		);

	UNearComponent* NearComponent = Cast<UNearComponent>(GetPlayerCharacterFromActorInfo()->GetNearComponent());

	TArray<AItemBase*> Items;
	bool bShouldUpdate = true;
	
	for (const FHitResult& HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetName());
			if (AItemBase* ItemBase = Cast<AItemBase>(HitActor))
			{
				Items.Add(ItemBase);
				UE_LOG(LogTemp, Warning, TEXT("Item Count : %d"), Items.Num());
			}
		}
	}
	
	for ( AItemBase* Item : Items)
	{
		bShouldUpdate = NearComponent->ShouldUpdate(Item);
		if (bShouldUpdate == false)
		{
			NearComponent->GetGroundItems().Empty();
			break;
		}
	}
	
	if (bShouldUpdate == true)
	{
		return;
	}
	else
	{
		for ( AItemBase* Item : Items)
		{
			if ( NearComponent->ShouldUpdate(Item) == false)
			{
				NearComponent->GetGroundItems().Add(Item);
			}
		}
		//NearComponent->UpdateInventory();
	}
}
