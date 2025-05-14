// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Interaction/GA_Interaction.h"

#include "Character/PlayerCharacter.h"
#include "Interface/InteractInterface.h"

UGA_Interaction::UGA_Interaction()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;

}

void UGA_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("GA_Interaction"));

	if (GetPlayerCharacterFromActorInfo()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : GA_Interaction"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : GA_Interaction"));

	}
	
	if (AActor* LookAtActor = Cast<AActor>(GetPlayerCharacterFromActorInfo()->GetLookAtActor()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *LookAtActor->GetActorLabel());

		if (LookAtActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_InteractWith(LookAtActor, GetPlayerCharacterFromActorInfo());
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *LookAtActor->GetActorLabel());
		}

		// IInteractInterface* InteractInterface = Cast<IInteractInterface>(LookAtActor);
		// InteractInterface->InteractWith(GetPlayerCharacterFromActorInfo());
	}

	EndAbility(Handle,ActorInfo, ActivationInfo, false, true);
}
