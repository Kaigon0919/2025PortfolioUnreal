// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KGBaseAbility.h"
#include "KGNormalAttackAbility.generated.h"

/**
 * 
 */

UCLASS()
class KGUNREALPORTFOLIO_API UKGNormalAttackAbility : public UKGBaseAbility
{
	GENERATED_BODY()
public:
	UKGNormalAttackAbility();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
