// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KGPlayerController.h"
#include <EnhancedInputSubsystems.h>

AKGPlayerController::AKGPlayerController()
{
}

void AKGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(true == IsLocalController())
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (true == IsValid(EnhancedInputSubSystem))
		{
			//EnhancedInputSubSystem->AddMappingContext(CDOGameInput->GetInputContext(), 0);

		}
	}

}
