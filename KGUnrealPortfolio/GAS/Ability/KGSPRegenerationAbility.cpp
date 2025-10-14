// Fill out your copyright notice in the Description page of Project Settings.


#include "KGSPRegenerationAbility.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>
#include <GAS/Effects/GE_SP.h>

UKGSPRegenerationAbility::UKGSPRegenerationAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	mSkillType = EKGSkillType::Passive;

}

void UKGSPRegenerationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AActor* owner = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ownerASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* ownerAttr = ownerASC->GetSet<UKGCharacterAttributeSet>();

	if (!ownerAttr)
	{
		// Ȱ�� ����Ƽ�� ���������� �ݵ�� EndAbility�� ȣ���ؾ� �Ѵ�.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	mSourceActor = owner;
	mSourceASC = ownerASC;
	mSpecHandle = Handle;

	mSourceActor->GetWorldTimerManager().SetTimer(mTimerHandle, FTimerDelegate::CreateUObject(this, &UKGSPRegenerationAbility::TickRegeneration), 1.f, true);

}

void UKGSPRegenerationAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	mSourceActor->GetWorldTimerManager().ClearTimer(mTimerHandle);
}

void UKGSPRegenerationAbility::TickRegeneration()
{
	const UKGCharacterAttributeSet* ownerAttr = mSourceASC->GetSet<UKGCharacterAttributeSet>();
	const float RegenrationValue = ownerAttr->GetSPRecovery();
	FGameplayEffectSpecHandle CostSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_SP::StaticClass(), GetAbilityLevel());
	CostSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.SP")), RegenrationValue);
	mSourceASC->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data);
}
