// Fill out your copyright notice in the Description page of Project Settings.


#include "KGBaseAbility.h"
#include "GAS/Attributes/KGCharacterAttributeSet.h"
#include <GAS/Effects/GE_Damage.h>
#include <GAS/Effects/GE_SP.h>
#include <GAS/Effects/GE_MP.h>

UKGBaseAbility::UKGBaseAbility()
{
}

bool UKGBaseAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, OptionalRelevantTags);
	if (false == result)
	{
		return false;
	}

	const AActor* SourceActor = ActorInfo->AvatarActor.Get();

	// SourceActor가 가지고 있는 AbilitySystemComponent를 얻어온다.
	const UAbilitySystemComponent* SourceASC = Cast<IAbilitySystemInterface>(SourceActor)->GetAbilitySystemComponent();
	const UKGCharacterAttributeSet* SourceAttr = SourceASC->GetSet<UKGCharacterAttributeSet>();
	if (!SourceAttr)
	{
		return false;
	}

	// 비용소모가 불가능할 경우 어빌리티 사용 중지.
	for (auto& Cost : CostDatas)
	{
		switch (Cost.Type)
		{
		case ECostType::MP:
			if (SourceAttr->GetMP() < Cost.Cost)
			{
				return false;
			}
			break;
		case ECostType::HP:
			if (SourceAttr->GetHP() <= Cost.Cost)
			{
				return false;
			}
			break;
		case ECostType::SP:
			if (SourceAttr->GetSP() < Cost.Cost)
			{
				return false;
			}
		}
	}
	return true;
}

void UKGBaseAbility::AddAbilityCost(ECostType Type, float Cost)
{
	FAbilityConstData	Result;
	Result.Type = Type;
	Result.Cost = Cost;
	CostDatas.Add(Result);
}

void UKGBaseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 필요하면 주석 제거.
	//const AActor* SourceActor = GetAvatarActorFromActorInfo();

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* SourceAttr = SourceASC->GetSet<UKGCharacterAttributeSet>();
	if (!SourceAttr)
	{
		return;
	}

	for (auto& Cost : CostDatas)
	{
		switch (Cost.Type)
		{
		case ECostType::MP:
		{
			FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_MP::StaticClass(), GetAbilityLevel());
			CostSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.MP")), -Cost.Cost);
			SourceASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data);
		}
			break;
		case ECostType::HP:
			{
				FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_Damage::StaticClass(), GetAbilityLevel());
				CostSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.Damage")), -Cost.Cost);
				SourceASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data);
			}
			break;
		case ECostType::SP:
			{
				FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_SP::StaticClass(), GetAbilityLevel());
				CostSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.SP")), -Cost.Cost);
				SourceASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data);
			}
			break;
		}
	}
}
