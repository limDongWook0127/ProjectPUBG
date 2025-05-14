// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LandscapeManager.generated.h"

class ALandscape;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API ULandscapeManager : public UObject
{
	GENERATED_BODY()

public:
	ULandscapeManager();
	
	void InitializeManager();
	
	// 랜드스케이프 설정
	void FindLandscape();
	
	// 랜드스케이프 설정
	void SetLandscape(ALandscape* Landscape);

	// 랜드스케이프의 바운딩 박스 가져오기
	FBox GetLandscapeBoundingBox() const;

	// 랜드스케이프의 크기(X, Y 길이) 가져오기
	FVector2D GetLandscapeSize() const;

	// 랜드스케이프 위의 랜덤 위치 가져오기
	FVector GetRandomLocationOnLandscape();
	

private:
	// 현재 사용 중인 랜드스케이프
	UPROPERTY()
	ALandscape* CurrentLandscape;
};
