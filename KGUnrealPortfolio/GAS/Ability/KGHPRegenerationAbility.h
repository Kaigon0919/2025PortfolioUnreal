// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/KGBaseAbility.h"
#include "KGHPRegenerationAbility.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGHPRegenerationAbility : public UKGBaseAbility
{
	GENERATED_BODY()
public:
	UKGHPRegenerationAbility();
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
