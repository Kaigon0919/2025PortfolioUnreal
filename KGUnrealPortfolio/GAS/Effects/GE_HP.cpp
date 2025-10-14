// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effects/GE_HP.h"
#include "GAS/Attributes/KGCharacterAttributeSet.h"

UGE_HP::UGE_HP()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UKGCharacterAttributeSet::GetHPAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Common.HP"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
