// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KGUtil.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EKGCharacterType : uint8
{
	None,
	Guardian UMETA(DisplayName = "Guardian"),
	Warrior UMETA(DisplayName = "Warrior"),
	Witch UMETA(DisplayName = "Witch"),
	MinionDuskMelee UMETA(DisplayName = "MinionDuskMelee"),
};