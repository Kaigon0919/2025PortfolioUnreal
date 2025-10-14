// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/KGBaseAbility.h"
#include "KGDamageReflectTriggerAbility.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGDamageReflectTriggerAbility : public UKGBaseAbility
{
	GENERATED_BODY()
public:
	UKGDamageReflectTriggerAbility();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
