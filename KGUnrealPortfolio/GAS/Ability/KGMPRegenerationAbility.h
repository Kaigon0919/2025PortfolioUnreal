// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/KGBaseAbility.h"
#include "KGMPRegenerationAbility.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGMPRegenerationAbility : public UKGBaseAbility
{
	GENERATED_BODY()
	
public:
	UKGMPRegenerationAbility();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

private:
	void TickRegeneration();
private:
	FTimerHandle	mTimerHandle;
	AActor* mSourceActor;
	UAbilitySystemComponent* mSourceASC;
	FGameplayAbilitySpecHandle	mSpecHandle;
	
};
