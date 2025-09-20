// Fill out your copyright notice in the Description page of Project Settings.


#include "KGCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

#define DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(Message, x)\
 Message += FString::Printf(TEXT("%s : %f / Base Value(%f)\n"),TEXT(#x), x.GetCurrentValue(), x.GetBaseValue())


UKGCharacterAttributeSet::UKGCharacterAttributeSet()
{
}

void UKGCharacterAttributeSet::GetDebugInfoString(FString& meesage) const
{
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,HP);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,HPMax);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,HPRecovery);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,MP);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,MPMax);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,MPRecovery);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,SP);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,SPMax);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,SPRecovery);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,Attack);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,AttackSpeed);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,Defence);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,Exp);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,Level);
	DEBUG_ATTRIBUTE_CURRENTBVALUE_STRING(meesage,Gold);

}

void UKGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetHPMax()));
	}

	else if (Data.EvaluatedData.Attribute == GetMPAttribute())
	{
		SetMP(FMath::Clamp(GetMP(), 0.0f, GetMPMax()));
	}

	else if (Data.EvaluatedData.Attribute == GetSPAttribute())
	{
		SetSP(FMath::Clamp(GetSP(), 0.0f, GetSPMax()));
	}
}

void UKGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHPMaxAttribute())
	{
		AdjustAttributeForMaxChange(HP, HPMax, NewValue, GetHPAttribute());
	}

	else if (Attribute == GetMPMaxAttribute())
	{
		AdjustAttributeForMaxChange(MP, MPMax, NewValue, GetMPAttribute());
	}

	else if (Attribute == GetSPMaxAttribute())
	{
		AdjustAttributeForMaxChange(SP, SPMax, NewValue, GetSPAttribute());
	}
}

bool UKGCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UKGCharacterAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}