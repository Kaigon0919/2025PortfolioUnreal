// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/KGBaseAbility.h"
#include "KGAttackUpAbility.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGAttackUpAbility : public UKGBaseAbility
{
	GENERATED_BODY()
public:
	UKGAttackUpAbility();
protected:
	float		mDuration = 10.f;
	float		mAttackUp = 1.2f;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
