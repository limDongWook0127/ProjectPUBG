// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLibrary/BaseFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EngineUtils.h"
#include "Abilities/GameplayAbility.h"
#include "Character/BaseCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UBaseAbilitySystemComponent* UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(AActor* Actor)
{
	check(Actor);
	return CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void UBaseFunctionLibrary::AddGameplayTagToActor(AActor* Actor, FGameplayTag Tag, bool bShouldReplicate)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (!ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->AddLooseGameplayTag(Tag);

		if (bShouldReplicate)
		{
			ASC->AddReplicatedLooseGameplayTag(Tag);
		}
	
	}
}

void UBaseFunctionLibrary::RemoveGameplayTagFromActor(AActor* Actor, FGameplayTag Tag, bool bShouldReplicate)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->RemoveLooseGameplayTag(Tag);

		if (bShouldReplicate)
		{
			ASC->RemoveReplicatedLooseGameplayTag(Tag);
		}
	}
}



bool UBaseFunctionLibrary::NativeActorHasTag(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	return ASC->HasMatchingGameplayTag(Tag);
}

void UBaseFunctionLibrary::BP_HasTag(AActor* Actor, FGameplayTag Tag, EBaseConfirmType& OutType)
{
	OutType = NativeActorHasTag(Actor, Tag) ? EBaseConfirmType::Yes : EBaseConfirmType::No;
}

bool UBaseFunctionLibrary::HasTag(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	return ASC->HasMatchingGameplayTag(Tag);
}

FString UBaseFunctionLibrary::ReadFile(FString FilePath)
{
	FString DirectoryPath = FPaths::ProjectContentDir();
	FString FullPath = DirectoryPath + "/" + FilePath;
	FString Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	if(File.FileExists(*FullPath))
	{
		FFileHelper::LoadFileToString(Result,*FullPath);
	}

	return Result;
}

TArray<APlayerCharacter*> UBaseFunctionLibrary::GetAllPlayersInWorld(UWorld* World)
{
	TArray<APlayerCharacter*> PlayerCharacters;

	if (World)
	{
		for (TActorIterator<APlayerCharacter> It(World); It; ++It)
		{
			APlayerCharacter* PlayerCharacter = *It;
			if (PlayerCharacter)
			{
				PlayerCharacters.Add(PlayerCharacter);
			}
		}
	}

	return PlayerCharacters;
}

FName UBaseFunctionLibrary::GetRowNameByIndex(UDataTable* DataTable, int32 Index)
{
	if (DataTable)
	{
		TArray<FName> RowNames = DataTable->GetRowNames();
		if (RowNames.IsValidIndex(Index))
		{
			return RowNames[Index];
		}
	}
	return FName();
}

int32 UBaseFunctionLibrary::GetRowIndexByName(UDataTable* DataTable, FName Name)
{
	if (DataTable == nullptr)
	{
		return -1;
	}

	TArray<FName> RowNames = DataTable->GetRowNames(); // 모든 RowName 가져오기
	
	for (int32 Index = 0; Index < RowNames.Num(); Index++)
	{
		if (RowNames[Index] == Name)
		{
			return Index; // 일치하는 인덱스 반환
		}
	}

	return -1;

}

FVector UBaseFunctionLibrary::DropLocation(APlayerCharacter* PlayerCharacter)
{
	FVector Start = PlayerCharacter->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Start Location : %f %f %f"), Start.X, Start.Y, Start.Z);
	FVector End = PlayerCharacter->GetActorLocation() - FVector(0.0f, 0.0f, 500.0f);
	//UE_LOG(LogTemp, Warning, TEXT("End Location : %f %f %f"), End.X, End.Y, End.Z);
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldDynamic);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(PlayerCharacter);
	FHitResult Hit;

	//UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green);

	if (UKismetSystemLibrary::LineTraceSingle(PlayerCharacter->GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Drop Location : %f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
		return Hit.Location;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Drop Location : 0"));
	return FVector(0, 0, 0);
}

