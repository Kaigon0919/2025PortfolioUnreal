// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_MP.h"
#include <GAS/Attributes/KGCharacterAttributeSet.h>

UGE_MP::UGE_MP()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UKGCharacterAttributeSet::GetMPAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.MP"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
