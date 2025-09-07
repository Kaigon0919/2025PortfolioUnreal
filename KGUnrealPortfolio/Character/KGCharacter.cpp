// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KGCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Input/KGInGameInput.h>

#include "Components/CapsuleComponent.h"
#include "Components/KGCharacterMovement.h"
#include "Animation/KGAnimInstance.h"
#include "KGAssetManager.h"

#include "KGUtil.h"

// Sets default values
AKGCharacter::AKGCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		mCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
		mCapsuleComponent->bVisualizeComponent = true;
		mCapsuleComponent->SetCollisionProfileName(TEXT("KGCharacter"));
		SetRootComponent(mCapsuleComponent);
	}

	{
		mMesh = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("Mesh"));
		mMesh->SetupAttachment(mCapsuleComponent);
		mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mMesh->SetCanEverAffectNavigation(false);
		mMesh->bRenderCustomDepth = true;
	}

	mMovement = CreateDefaultSubobject<UKGCharacterMovement>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(GetRootComponent());
}

void AKGCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (characterType != EKGCharacterType::None)
	{
		FKGAnimInfo* AnimInfo = UKGAssetManager::Get().FindDataTableRow<FKGAnimInfo>(TEXT("DTCharacterAnimInfo"), characterType);
		if (nullptr != AnimInfo)
		{
			mMesh->SetSkeletalMeshAsset(AnimInfo->SkeletalMesh);
			mMesh->SetAnimInstanceClass(AnimInfo->AnimIntanceClass);
		}
	}

}

void AKGCharacter::PreInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	mAnimInst = Cast<UKGAnimInstance>(mMesh->GetAnimInstance());
}

// Called when the game starts or when spawned
void AKGCharacter::BeginPlay()
{
	Super::BeginPlay();

	//check(mAnimInst); 
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
			const UKGInGameInput* CDOGameInput = GetDefault<UKGInGameInput>();
			mEnhancedInputSubSystem->AddMappingContext(CDOGameInput->GetInputContext(), 0);

			inputComp->BindAction(CDOGameInput->GetInputMoveAction(), ETriggerEvent::Triggered, this, &AKGCharacter::MoveAction);
			inputComp->BindAction(CDOGameInput->GetInputRotateAction(), ETriggerEvent::Triggered, this, &AKGCharacter::RotateView);
			inputComp->BindAction(CDOGameInput->GetInputMouseRotateAction(), ETriggerEvent::Triggered, this, &AKGCharacter::MouseRotateView);
			inputComp->BindAction(CDOGameInput->GetInputAttackAction(), ETriggerEvent::Started, this, &AKGCharacter::AttackKey);

			inputComp->BindAction(CDOGameInput->GetInputSkill1Action(), ETriggerEvent::Started, this, &AKGCharacter::Skill1Action);
			inputComp->BindAction(CDOGameInput->GetInputSkill2Action(), ETriggerEvent::Started, this, &AKGCharacter::Skill2Action);
			inputComp->BindAction(CDOGameInput->GetInputSkill3Action(), ETriggerEvent::Started, this, &AKGCharacter::Skill3Action);
		}
	}

}

void AKGCharacter::MoveAction(const FInputActionValue& value)
{
	FVector axisValue = value.Get<FVector>();

	const FRotator rotation = this->Controller->GetControlRotation();
	const FRotator yawRotation(0.0, rotation.Yaw, 0.0);

	const FVector forwardDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	FString DebugMessage = FString::Printf(TEXT("x : %f / y : %f"), axisValue.X, axisValue.Y);
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, DebugMessage);

	mMovement->AddInputVector(forwardDir * axisValue.X, false);
	mMovement->AddInputVector(rightDir * axisValue.Y, false);

	float dir = 0.f;
	if (axisValue.Size() != 0.0f)
	{
		FVector normalVector = axisValue;
		normalVector.Normalize();
		float atan2 = FMath::Atan2(normalVector.Y, normalVector.X);
		float AngleDegree = FMath::RadiansToDegrees(atan2);
		dir = AngleDegree;
	}

	if (IsValid(mAnimInst))
	{
		mAnimInst->SetDir(dir);
	}
}

void AKGCharacter::RotateView(const FInputActionValue& value)
{
	//UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
	float actionValue = value.Get<float>();
	AddControllerYawInput(actionValue);
}

void AKGCharacter::MouseRotateView(const FInputActionValue& value)
{
	//UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
	FVector axisValue = value.Get<FVector>();
	AddControllerYawInput(axisValue.X);
	AddControllerPitchInput(-axisValue.Y);
}

void AKGCharacter::AttackKey()
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

	NormalAttackAction();
}

void AKGCharacter::Skill1Action(const FInputActionValue& value)
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

}

void AKGCharacter::Skill2Action(const FInputActionValue& value)
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

}

void AKGCharacter::Skill3Action(const FInputActionValue& value)
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

}

void AKGCharacter::NormalAttackAction()
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

}

void AKGCharacter::NormalAttack()
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

float AKGCharacter::GetCapsuleHalfHeight() const
{
	return mCapsuleComponent->GetScaledCapsuleHalfHeight();
}