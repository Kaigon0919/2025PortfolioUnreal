// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KGBaseAbility.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECostType : uint8
{
	HP,
	MP,
	SP,
};

USTRUCT(BlueprintType)
struct FAbilityConstData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	ECostType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Cost;
};

UCLASS()
class KGUNREALPORTFOLIO_API UKGBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UKGBaseAbility();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<FAbilityConstData> CostDatas;
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	void AddAbilityCost(ECostType Type, float Cost);
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
