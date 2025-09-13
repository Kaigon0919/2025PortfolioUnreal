// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KGInGameInput.generated.h"

#define KGADDACTION(x) private: TObjectPtr<UInputAction> mInput##x; public: const UInputAction* GetInput##x() const { return mInput##x; }
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGInGameInput : public UObject
{
	GENERATED_BODY()
public:
	UKGInGameInput();
private:
	TObjectPtr<UInputMappingContext> mInputContext;
public:
	const UInputMappingContext* GetInputContext() const { return mInputContext; }

	KGADDACTION(MoveAction);
	KGADDACTION(RotateAction);
	KGADDACTION(AttackAction);
	KGADDACTION(MouseRotateAction);
	KGADDACTION(MouseViewDistanceAction);
	KGADDACTION(Skill1Action);
	KGADDACTION(Skill2Action);
	KGADDACTION(Skill3Action);
	KGADDACTION(ChangeInputMode);
};
