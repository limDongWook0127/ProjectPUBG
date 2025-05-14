// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UBaseAbilitySystemComponent;
class UBaseAttributeSet;
class UDataAsset_StartupBase;
class UGameplayEffect;

UCLASS()
class PUBG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	


public:
	// Sets default values for this character's properties
	ABaseCharacter(const class FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual bool IsAlive() const;
	/**
	* Getters for attributes from GDAttributeSetBase
	**/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMoveSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMagazine() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxMagazine() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetArmor() const;
		
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetKillCount() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRank() const;
	
protected:
	// TWeakObjectPtrs 대신 UPROPERTY() 하드 참조를 갖거나 참조가 전혀 없고 호출만 할 수도 있습니다.
	// GetAbilitySystem()을 실행하고 PlayerState 또는 하위 클래스에서 읽을 수 있는 GetAttributeSetBase()를 만듭니다.
	// 사용하기 전에 포인터가 유효한지 테스트했는지 확인하세요.
	// 여기서는 공유된 하드 참조를 원하지 않았고 가져오는 추가 함수 호출도 원하지 않았기 때문에 TWeakObjectPtrs를 선택했습니다.
	// 이 기본 클래스에서 참조할 때마다 PlayerState 또는 하위 클래스의 ASC/AttributeSet입니다.

	TWeakObjectPtr<class UBaseAbilitySystemComponent> BaseAbilitySystemComponent;
	TWeakObjectPtr<class UBaseAttributeSet> BaseAttributeSet;
	
	//동기식으로 데이터를 불러온다
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "StartupData")
	TSoftObjectPtr<UDataAsset_StartupBase> StartupData;

	virtual void PossessedBy(AController* NewController) override;

public:
	//UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const { return BaseAbilitySystemComponent; }
	UBaseAttributeSet* GetBaseAttributeSet() const { return BaseAttributeSet.Get(); }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GE_HealthRegen;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GE_StaminaDecay;

public:
	void ApplyHealthRegenEffect() const;
	void RemoveHealthRegenEffect() const;
	void ApplyStaminaDecayEffect() const;
	void RemoveStaminaDecayEffect() const;

	/**
	* 속성에 대한 설정자. Respawning과 같은 특별한 경우에만 이것을 사용하고, 그렇지 않으면 GE를 사용하여 속성을 변경하십시오.
	* 이는 속성의 기본 값을 변경합니다.
	*/

	virtual void SetHealth(float Health);
	virtual void SetStamina(float Stamina);
	virtual void SetArmor(float Armor);
	virtual void SetMoveSpeed(float MoveSpeed);
	virtual void SetMagazine(float Magazine);
	virtual void SetMaxMagazine(float MaxMagazine);
	virtual void SetKillCount(float KillCount);
	virtual void SetRank(float Rank);

};


