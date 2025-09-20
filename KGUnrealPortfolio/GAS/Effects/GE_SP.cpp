// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_SP.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>

UGE_SP::UGE_SP()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UKGCharacterAttributeSet::GetSPAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.SP"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
