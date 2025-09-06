// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KGCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AKGCharacter::AKGCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (false == IsValid(playerController))
	{
		return;
	}

	if (true == playerController->IsLocalController())
	{
		mEnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
		UEnhancedInputComponent* inputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (IsValid(inputComp))
		{
			//const UGameInput* CDOGameInput = GetDefault<UGameInput>();
			//inputComp->BindAction(CDOGameInput->GetInputMoveAction(), ETriggerEvent::Triggered, this, &AKGCharacter::MoveAction);
			//inputComp->BindAction(CDOGameInput->GetInputRotateAction(), ETriggerEvent::Triggered, this, &AKGCharacter::RotateView);
			//inputComp->BindAction(CDOGameInput->GetInputJumpAction(), ETriggerEvent::Triggered, this, &AKGCharacter::JumpAction);
			//inputComp->BindAction(CDOGameInput->GetInputMouseRotateAction(), ETriggerEvent::Triggered, this, &AKGCharacter::MouseRotateView);
			//inputComp->BindAction(CDOGameInput->GetInputAttackAction(), ETriggerEvent::Started, this, &AKGCharacter::AttackKey);

			//inputComp->BindAction(CDOGameInput->GetInputSkill1Action(), ETriggerEvent::Started, this, &AKGCharacter::Skill1Action);
			//inputComp->BindAction(CDOGameInput->GetInputSkill2Action(), ETriggerEvent::Started, this, &AKGCharacter::Skill2Action);
			//inputComp->BindAction(CDOGameInput->GetInputSkill3Action(), ETriggerEvent::Started, this, &AKGCharacter::Skill3Action);
		}
	}

}

void AKGCharacter::MoveAction(const FInputActionValue& value)
{
}

void AKGCharacter::RotateView(const FInputActionValue& value)
{
}

void AKGCharacter::JumpAction(const FInputActionValue& value)
{
}

void AKGCharacter::MouseRotateView(const FInputActionValue& value)
{
}

void AKGCharacter::AttackKey()
{
	NormalAttackAction();
}

void AKGCharacter::Skill1Action(const FInputActionValue& value)
{
}

void AKGCharacter::Skill2Action(const FInputActionValue& value)
{
}

void AKGCharacter::Skill3Action(const FInputActionValue& value)
{
}

void AKGCharacter::NormalAttackAction()
{
}

void AKGCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	mAlliance = static_cast<EKGAlliance>(TeamID.GetId());
}

FGenericTeamId AKGCharacter::GetGenericTeamId() const
{
	return static_cast<uint8>(mAlliance);
}

