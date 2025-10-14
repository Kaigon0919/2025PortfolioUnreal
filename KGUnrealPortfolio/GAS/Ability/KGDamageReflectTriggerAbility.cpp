// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/KGDamageReflectTriggerAbility.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>
#include <Components/KGCharacterMovement.h>
#include <Character/KGCharacter.h>

UKGDamageReflectTriggerAbility::UKGDamageReflectTriggerAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	mSkillType = EKGSkillType::Passive;
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Custom.Buff.DamageReflect")));

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Event.Hit"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UKGDamageReflectTriggerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* owner = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ownerASC = GetAbilitySystemComponentFromActorInfo();
	const UKGCharacterAttributeSet* ownerAttr = ownerASC->GetSet<UKGCharacterAttributeSet>();

	//FGameplayAbilityTargetData_SingleTargetHit* HitData = static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(TriggerEventData->TargetData.Data[0].Get());
	
	const AKGCharacter* instigator = TriggerEventData->Instigator ? Cast<AKGCharacter>(TriggerEventData->Instigator.Get()) : nullptr;
	if (instigator)
	{
		UKGCharacterMovement* Movement = Cast<UKGCharacterMovement>(instigator->GetMovementComponent());
		if (Movement)
		{
			FVector KnockbackVector = instigator->GetActorLocation() - owner->GetActorLocation();
			KnockbackVector.Z = 0.0f;
			KnockbackVector.Normalize();
			FVector rotateVector = KnockbackVector.Cross(FVector::UpVector);

			KnockbackVector = KnockbackVector.RotateAngleAxis(40.f, rotateVector);

			Movement->AddKnockbackImpulse(KnockbackVector * 1000.f);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
