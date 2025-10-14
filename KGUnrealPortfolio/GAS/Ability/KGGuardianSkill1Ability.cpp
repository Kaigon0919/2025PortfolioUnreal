// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/KGGuardianSkill1Ability.h"
#include "KGGuardianSkill1Ability.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>
#include <AbilitySystemBlueprintLibrary.h>
#include <GAS/Effects/GE_Damage.h>
#include <Components/KGCharacterMovement.h>

UKGGuardianSkill1Ability::UKGGuardianSkill1Ability()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AddAbilityCost(ECostType::MP, 15.f);

	mSkillType = EKGSkillType::Active;
}

void UKGGuardianSkill1Ability::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	FVector forwardVec = owner->GetActorForwardVector();
	TArray<AActor*> ignoreActor;
	ignoreActor.Add(owner);

	FCollisionQueryParams param;
	param.AddIgnoredActors(ignoreActor);
	param.bTraceComplex = false;

	FVector startPosition = owner->GetActorLocation();
	FVector endPosition = startPosition;
	const float radius = 500.f;

	const float checkCollisionRadian = FMath::Cos(FMath::DegreesToRadians(90.0f));
	FCollisionShape shape = FCollisionShape::MakeSphere(radius);

	TArray<FHitResult> hitResults;
	bool collision = GetWorld()->SweepMultiByChannel(hitResults, startPosition, endPosition, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, shape, param);
	if (collision)
	{
		const float AttackDamage = ownerAttr->GetAttack();
		TArray<FHitResult>::TIterator iter = hitResults.CreateIterator();
		for (; iter; ++iter)
		{
			FHitResult hit = *iter;
			FVector targetPosition = hit.GetActor()->GetActorLocation();
			FVector dir = targetPosition - startPosition;
			dir.Normalize();

			AActor* hittedActor = hit.GetActor();
			APawn* DestPawn = Cast<APawn>(hittedActor);
			if (!DestPawn)
			{
				continue;
			}

			UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(DestPawn);
			const UKGCharacterAttributeSet* targetAttr = targetASC->GetSet<UKGCharacterAttributeSet>();

			float	Defense = targetAttr->GetDefence();
			float	Dmg = AttackDamage - Defense;
			Dmg = FMath::Max(1.f, Dmg);

			FGameplayEffectContextHandle ContextHandle = MakeEffectContext(Handle, ActorInfo);
			ContextHandle.AddHitResult(hit);

			FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(UGE_Damage::StaticClass(), 1.f);
			DamageSpecHandle.Data->SetContext(ContextHandle);
			DamageSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.Damage")), -Dmg);
			ownerASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), targetASC);

			UKGCharacterMovement* Movement = Cast<UKGCharacterMovement>(DestPawn->GetMovementComponent());
			if (Movement)
			{
				FVector KnockbackVector = dir;
				KnockbackVector.Z = 0.0f;
				KnockbackVector.Normalize();
				FVector rotateVector = KnockbackVector.Cross(FVector::UpVector);
					
				KnockbackVector= KnockbackVector.RotateAngleAxis(40.f, rotateVector);

				Movement->AddKnockbackImpulse(KnockbackVector * 3000.f);
			}
			
		}
	}

	FGameplayCueParameters	Param;
	Param.Instigator = GetAvatarActorFromActorInfo();
	Param.EffectCauser = GetOwningActorFromActorInfo();
	Param.Location = owner->GetActorLocation();

	ownerASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Guardian.Skill1")), Param);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

#if WITH_EDITOR

	// 디버그용
	FColor DebugColor = collision ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), startPosition, radius, 12, DebugColor, false, 0.1f);
#endif
}
