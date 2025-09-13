// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "KGCharacterAttributeSet.generated.h"

#define	ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


#define KG_ATTRIBUTE_MACRO(ClassName, PropertyName)	\
	FGameplayAttributeData PropertyName;			\
public: ATTRIBUTE_ACCESSORS(ClassName, PropertyName);

UCLASS()
class KGUNREALPORTFOLIO_API UKGCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UKGCharacterAttributeSet();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HPMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HPRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MPMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MPRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData SP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData SPMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData SPRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute", Meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Exp;

public:
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, HP		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, HPMax		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, HPRecovery   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, MP		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, MPMax		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, MPRecovery   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, SP		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, SPMax		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, SPRecovery   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, Attack	   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, Defence	   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, AttackSpeed  );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, Exp		   );
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, Level			);
	ATTRIBUTE_ACCESSORS(UKGCharacterAttributeSet, Gold		   );

	void GetDebugInfoString(FString& meesage)const;
};
