// Fill out your copyright notice in the Description page of Project Settings.


#include "KGCharacterMovement.h"
#include "Character/KGCharacter.h"

UKGCharacterMovement::UKGCharacterMovement()
{
	PrimaryComponentTick.bCanEverTick = true;

	mGravityDir = FVector(0.f, 0.f, -1.f);
	mGravity = 980.f;
	mApplyGravity = true;
}

bool UKGCharacterMovement::CheckGround()
{
	const AKGCharacter* character = Cast<AKGCharacter>(GetOwner());
	if (false == IsValid(character))
	{
		return false;
	}

	const float groundCheckDistance = 0.0f;
	const float checkRadius = 5.f;
	FHitResult Hit;
	FVector Start = GetActorLocation();
	Start.Z -= character->GetCapsuleHalfHeight();
	FVector End = Start - FVector(0, 0, groundCheckDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepSingleByChannel(Hit,Start,End,FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(checkRadius), Params);
#if WITH_EDITOR

	// 디버그용
	FColor DebugColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), bHit ? Hit.ImpactPoint : End, checkRadius, 12, DebugColor, false, 0.1f);
#endif

	return bHit;
}

bool UKGCharacterMovement::IsMovingOnGround() const
{
	return mIsGround;
}

void UKGCharacterMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	UPawnMovementComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	const AController* Controller = PawnOwner->GetController();
	if (Controller && Controller->IsLocalController())
	{
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() == true || Controller->IsFollowingAPath() == false || NavMovementProperties.bUseAccelerationForPaths)
		{
			ApplyControlInputToVelocity(DeltaTime);
		}
		// if it's not player controller, but we do have a controller, then it's AI
		// (that's not following a path) and we need to limit the speed
		else if (IsExceedingMaxSpeed(MaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		LimitWorldBounds();
		bPositionCorrected = false;

		// Move actor
		FVector Delta = Velocity * DeltaTime;
		if (mApplyGravity)
		{
			Delta += (mGravity * mGravityDir * DeltaTime);
		}

		if (!Delta.IsNearlyZero(1e-6f))
		{
			const FVector OldLocation = UpdatedComponent->GetComponentLocation();
			const FQuat Rotation = UpdatedComponent->GetComponentQuat();

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, DeltaTime, Delta);
				// Try to slide the remaining distance along the surface.
				SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}

			// Update velocity
			// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
			if (!bPositionCorrected)
			{
				const FVector NewLocation = UpdatedComponent->GetComponentLocation();
				Velocity = ((NewLocation - OldLocation) / DeltaTime);
			}
		}

		mIsGround = CheckGround();
		// Finalize
		UpdateComponentVelocity();
	}
}
