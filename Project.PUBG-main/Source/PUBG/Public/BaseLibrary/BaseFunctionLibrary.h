// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "BaseLibrary/BaseEnumType.h"
#include "BaseFunctionLibrary.generated.h"

class UBaseAbilitySystemComponent;
class ABaseCharacter;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PUBG_API UBaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	
public:
	//UBlueprintFunctionLibrary
	static UBaseAbilitySystemComponent* NativeGetBaseAbilitySystemComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "FuntionLibary")
	static void AddGameplayTagToActor(AActor* Actor, FGameplayTag Tag, bool bShouldReplicate);

	UFUNCTION(BlueprintCallable, Category = "FuntionLibary")
	static void RemoveGameplayTagFromActor(AActor* Actor, FGameplayTag Tag, bool bShouldReplicate);
	
	static bool NativeActorHasTag(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibary", meta = (Display = "Does Actor Has Tag", ExpandEnumAsExecs = "OutType"))
	static void BP_HasTag(AActor* Actor, FGameplayTag Tag, EBaseConfirmType& OutType);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibary")
	static bool HasTag(AActor* Actor, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibary")
	static FString ReadFile(FString FilePath);
	
	UFUNCTION(BlueprintCallable, Category = "FunctionLibary")
	static TArray<APlayerCharacter*> GetAllPlayersInWorld(UWorld* World);

	// 이준수
	UFUNCTION(BlueprintCallable, Category = "FunctionLibary")
	static FName GetRowNameByIndex(UDataTable* DataTable, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "FunctionLibary")
	static int32 GetRowIndexByName(UDataTable* DataTable, FName Name);
	UFUNCTION(BlueprintCallable, Category = "FunctionLibary")
	static FVector DropLocation(APlayerCharacter* PlayerCharacter); // 현재 자기 위치에서 바닥에 아이템 떨어뜨릴 위치 
};
