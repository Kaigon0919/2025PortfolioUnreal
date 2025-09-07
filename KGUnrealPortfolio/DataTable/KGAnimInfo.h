// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include <KGCharacterType.h>
#include "KGAnimInfo.generated.h"

/**
 * 
 */

class UKGAnimInstance;

USTRUCT(BlueprintType)
struct FKGAnimInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UKGAnimInstance> AnimIntanceClass;
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UAnimSequence>> SequenceMap;
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UBlendSpace>> BlendSpaceMap;
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UAnimMontage>> MontageMap;
};