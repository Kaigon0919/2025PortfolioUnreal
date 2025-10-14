// Fill out your copyright notice in the Description page of Project Settings.


#include "KGAttackUpAbility.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>
#include "GAS/Effects/GE_AttackUpPercent.h"

UKGAttackUpAbility::UKGAttackUpAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AddAbilityCost(ECostType::MP, 30.f);
	mSkillType = EKGSkillType::Active;
}

void UKGAttackUpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* owner = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ownerASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* ownerAttr = ownerASC->GetSet<UKGCharacterAttributeSet>();

	if (!ownerAttr)
	{
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FGameplayEffectSpecHandle	AttackUpSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_AttackUpPercent::StaticClass(), 1.f);
	AttackUpSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.Duration")), mDuration);
	//AttackUpSpecHandle.Data->SetDuration(mDuration, false);
	AttackUpSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.Attack")),mAttackUp);
	ownerASC->ApplyGameplayEffectSpecToSelf(*AttackUpSpecHandle.Data);
	//ownerASC->FindActiveGameplayEffectHandle

	// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}