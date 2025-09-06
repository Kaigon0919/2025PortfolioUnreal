// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KGCharacterType.h"
#include "InputAction.h"
#include "KGAlliance.h"
#include "GenericTeamAgentInterface.h"

#include "KGCharacter.generated.h"

UCLASS()
class KGUNREALPORTFOLIO_API AKGCharacter : public APawn, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> mCapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpringArmComponent> mSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> mCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent>	mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EKGCharacterType characterType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UKGAnimInstance> mAnimInst;


#pragma region Input
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> mEnhancedInputSubSystem;
public:
	/*virtual*/ void MoveAction(const FInputActionValue& value);
	/*virtual*/ void RotateView(const FInputActionValue& value);
	/*virtual*/ void JumpAction(const FInputActionValue& value);
	/*virtual*/ void MouseRotateView(const FInputActionValue& value);
	/*virtual*/ void AttackKey();

	/*virtual*/ void Skill1Action(const FInputActionValue& value);
	/*virtual*/ void Skill2Action(const FInputActionValue& value);
	/*virtual*/ void Skill3Action(const FInputActionValue& value);
	/*virtual */void NormalAttackAction();
#pragma endregion

#pragma region Alliance
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	EKGAlliance mAlliance;
public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	inline EKGAlliance GetAlliance() const { return mAlliance; }
	inline void SetAlliance(EKGAlliance Alliance) { mAlliance = Alliance; }
#pragma endregion

};
