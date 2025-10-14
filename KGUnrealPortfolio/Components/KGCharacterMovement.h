// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "KGCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGCharacterMovement : public UFloatingPawnMovement
{
	GENERATED_BODY()
public:
	UKGCharacterMovement();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool	mApplyGravity;
	float	mGravity;
	FVector	mGravityDir;
	FVector	mKnockbackVelocity;
	float	mKnockbackDamping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool mIsGround = false;
	bool mIsEnableCheckGround = false;
public:
	void EnableGravity(bool Enable) { mApplyGravity = Enable; }
private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool CheckGround();

public:
	virtual bool IsMovingOnGround() const override;
	void AddKnockbackImpulse(const FVector& Impulse);
	void SetKnockbackDamping(float Damping) { mKnockbackDamping = Damping; }
};
