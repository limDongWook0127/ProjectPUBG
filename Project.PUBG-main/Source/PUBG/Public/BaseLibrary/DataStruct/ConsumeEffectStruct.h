#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConsumeEffectStruct.generated.h"


USTRUCT(BlueprintType)
struct FConsumeEffectStruct : public FTableRowBase
{
	GENERATED_BODY()

	FConsumeEffectStruct()
	{
		Name = NAME_None;
		// SkeletalMesh = nullptr;
		// StaticMesh = nullptr;
		// AnimationAsset = nullptr;
		BP_StaticMeshActor = nullptr;
		UseTime = -1.0f;
		Level = 1;
		StandMontage = nullptr;
		ProneMontage = nullptr;
		GameplayEffectClass = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// USkeletalMesh* SkeletalMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// UStaticMesh* StaticMesh;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// UAnimSequence* AnimationAsset; // 아이템 사용 시 아이템 애니메이션 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> BP_StaticMeshActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* StandMontage; // 아이템 사용 시 캐릭터 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ProneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass; // 아이템 사용 시 캐릭터 애니메이션 몽타주 
};
