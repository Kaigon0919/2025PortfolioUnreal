// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include <GAS/Ability/KGBaseAbility.h>
#include "KGCharacterStatusInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FKGCharacterStatusInfo : public FTableRowBase
{
	GENERATED_BODY()
public:				   
	UPROPERTY(EditAnywhere)
	float HP		   ;
	UPROPERTY(EditAnywhere)
	float HPRecovery   ;
	UPROPERTY(EditAnywhere)
	float MP		   ;
	UPROPERTY(EditAnywhere)
	float MPRecovery   ;
	UPROPERTY(EditAnywhere)
	float SP		   ;
	UPROPERTY(EditAnywhere)
	float SPRecovery   ;
	UPROPERTY(EditAnywhere)
	float Attack	   ;
	UPROPERTY(EditAnywhere)
	float AttackSpeed  ;
	UPROPERTY(EditAnywhere)
	float Defence	   ;
	UPROPERTY(EditAnywhere)
	int32 Level;
	UPROPERTY(EditAnywhere)
	int32 Exp		   ;
	UPROPERTY(EditAnywhere)
	int32 Gold		   ;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKGBaseAbility> SkillAbility1;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKGBaseAbility> SkillAbility2;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKGBaseAbility> SkillAbility3;
};
