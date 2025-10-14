// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_AttackUpPercent.h"
#include<GAS/Attributes/KGCharacterAttributeSet.h>

UGE_AttackUpPercent::UGE_AttackUpPercent()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat	DurationCaller;
	DurationCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.Duration"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationCaller);

	FGameplayModifierInfo	Info;
	{
		FSetByCallerFloat	Caller;
		Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.Attack"));

		Info.Attribute = UKGCharacterAttributeSet::GetAttackAttribute();
		Info.ModifierOp = EGameplayModOp::Multiplicitive;
		Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);
		Modifiers.Add(Info);
	}

}
