// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/KGInGameInput.h"

#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/InputAction.h"

#define LOAD_INPUT_ASSET(x,y) \
static ConstructorHelpers::FObjectFinder<UInputAction> Input##x(TEXT(y)); \
    checkf(Input##x.Succeeded(), TEXT("Failed to load InputAction asset [File : %s , Path : %s]"),TEXT(#x), TEXT(y)); \
	mInput##x = Input##x.Object;
UKGInGameInput::UKGInGameInput()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/_KGContents/Input/IMInGameInput.IMInGameInput'"));
	checkf(InputContext.Succeeded(), TEXT("Failed to load InputContext asset [File : InputContext]"));
	mInputContext = InputContext.Object;

	LOAD_INPUT_ASSET(MoveAction,		"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/IAMove.IAMove'");
	LOAD_INPUT_ASSET(RotateAction,		"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/IARotate.IARotate'");
	LOAD_INPUT_ASSET(AttackAction,		"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/IAAttack.IAAttack'");
	LOAD_INPUT_ASSET(MouseRotateAction, "/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/IAMouseViewRotate.IAMouseViewRotate'");
	LOAD_INPUT_ASSET(MouseViewDistanceAction, "/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/IAMouseViewDistance.IAMouseViewDistance'");
	LOAD_INPUT_ASSET(Skill1Action,		"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/Skill1.Skill1'");
	LOAD_INPUT_ASSET(Skill2Action,		"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/Skill2.Skill2'");
	LOAD_INPUT_ASSET(Skill3Action,		"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/Skill3.Skill3'");
	LOAD_INPUT_ASSET(ChangeInputMode,	"/Script/EnhancedInput.InputAction'/Game/_KGContents/Input/IAChangeInputMode.IAChangeInputMode'");
}
