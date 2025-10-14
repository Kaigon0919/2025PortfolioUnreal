// Fill out your copyright notice in the Description page of Project Settings.


#include "KGHPRegenerationAbility.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>
#include <GAS/Effects/GE_HP.h>

UKGHPRegenerationAbility::UKGHPRegenerationAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	mSkillType = EKGSkillType::Passive;
}

void UKGHPRegenerationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AActor* owner = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ownerASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* ownerAttr = ownerASC->GetSet<UKGCharacterAttributeSet>();

	if (!ownerAttr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	mSourceActor = owner;
	mSourceASC = ownerASC;
	mSpecHandle = Handle;

	mSourceActor->GetWorldTimerManager().SetTimer(mTimerHandle, FTimerDelegate::CreateUObject(this, &UKGHPRegenerationAbility::TickRegeneration), 1.f, true);

}

void UKGHPRegenerationAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	mSourceActor->GetWorldTimerManager().ClearTimer(mTimerHandle);
}

void UKGHPRegenerationAbility::TickRegeneration()
{
	const UKGCharacterAttributeSet* ownerAttr = mSourceASC->GetSet<UKGCharacterAttributeSet>();
	const float RegenrationValue = ownerAttr->GetHPRecovery();
	FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_HP::StaticClass(), GetAbilityLevel());
	CostSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.HP")), RegenrationValue);
	mSourceASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data);
}
