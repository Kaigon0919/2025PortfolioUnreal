// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/KGDamageReflectAbility.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>
#include "KGDamageReflectTriggerAbility.h"
#include <GAS/Effects/KGDamageReflectStateEffect.h>

UKGDamageReflectAbility::UKGDamageReflectAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	mSkillType = EKGSkillType::Passive;
}

void UKGDamageReflectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* owner = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ownerASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* ownerAttr = ownerASC->GetSet<UKGCharacterAttributeSet>();

	mTriggerAbilitySpecHandle = ownerASC->GiveAbility(FGameplayAbilitySpec(UKGDamageReflectTriggerAbility::StaticClass()));

	FGameplayEffectSpecHandle	DamageReflectStateHandle = MakeOutgoingGameplayEffectSpec(UKGDamageReflectStateEffect::StaticClass(), 1.f);
	DamageReflectStateHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Custom.Buff.DamageReflect")));
	mDamageReflectStateHandle = ownerASC->ApplyGameplayEffectSpecToSelf(*DamageReflectStateHandle.Data);
}

void UKGDamageReflectAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AActor* owner = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ownerASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* ownerAttr = ownerASC->GetSet<UKGCharacterAttributeSet>();

	ownerASC->ClearAbility(mTriggerAbilitySpecHandle);
	ownerASC->RemoveActiveGameplayEffect(mDamageReflectStateHandle);
}
