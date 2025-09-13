// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KGAnimInstance.h"
#include <Character/KGCharacter.h>
#include <Components/KGCharacterMovement.h>
#include "KGAssetManager.h"

UKGAnimInstance::UKGAnimInstance()
{
	OnMontageEnded.AddDynamic(this, &UKGAnimInstance::MontageEnd);
}

void UKGAnimInstance::PostInitProperties()
{
	Super::PostInitProperties();

	if (mCharacterType != EKGCharacterType::None)
	{
		mAnimInfo = UKGAssetManager::Get().FindDataTableRow<FKGAnimInfo>(TEXT("DTCharacterAnimInfo"), mCharacterType);
		if (nullptr != mAnimInfo)
		{

			mAttackMontage = FindAnimMontage(TEXT("NormalAttack"));
			mLandRecoveryMontage = FindAnimMontage(TEXT("LandRecovery"));
		}
	}
}

void UKGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UKGAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UKGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AKGCharacter* Character = Cast<AKGCharacter>(TryGetPawnOwner());
	if (!IsValid(Character))
	{
		return;
	}

	// KGCharacter 전용 Movement 필요.
	const UKGCharacterMovement* Movement = Cast<UKGCharacterMovement>(Character->GetMovementComponent());
	if (!IsValid(Movement))
	{
		return;
	}

	mMoveSpeed = Movement->Velocity.Size();
	mIsGround = Movement->IsMovingOnGround();
	mIsFalling = !mIsGround; // 수영 등은 고민하지 않는다 공중아니면 무조건 하늘
}

void UKGAnimInstance::AnimNotify_Attack()
{
	AActor* owner = GetOwningActor();
	AKGCharacter* character = Cast<AKGCharacter>(owner);
	if (IsValid(character))
	{
		character->NormalAttack();
		return;
	}
}

void UKGAnimInstance::AnimNotify_ReadyCombo()
{
	mIsReadyCombo = true;
	OnReadyComboDelegate.Broadcast();
}

void UKGAnimInstance::AnimNotify_LandToLocoTransitionStart()
{
	mLandRecoveryAlpha = 1.0f;
	if (IsValid(mLandRecoveryMontage))
	{
		Montage_SetPosition(mLandRecoveryMontage, 0.f);
		Montage_Play(mLandRecoveryMontage);
	}
}

void UKGAnimInstance::CustomAnimNotify_SkillActive(UAnimSequenceBase* Animation)
{
	OnSkillActiveDelegate.Broadcast(Animation);
}

void UKGAnimInstance::MontageEnd(UAnimMontage* Montage, bool Interrupted)
{
	mIsReadyCombo = false;

	if (mAttackMontage == Montage && !Interrupted)
	{
		mAttackSectionIndex = 0;
	}

	if (mLandRecoveryMontage == Montage && !Interrupted)
	{
		mLandRecoveryAlpha = 0.0f;
	}
}

bool UKGAnimInstance::PlayAttackMontage(float PlayRate)
{
	if (!IsValid(mAttackMontage))
	{
		return false;
	}

	if (!mIsGround)
	{
		return false;
	}

	const int32 maxComboIndex = mAttackMontage->CompositeSections.Num();
	if (maxComboIndex <= mAttackSectionIndex)
	{
		return false;
	}

	if (mIsReadyCombo)
	{
		mIsReadyCombo = false;
		mAttackSectionIndex = (mAttackSectionIndex + 1) % maxComboIndex;
		const FName nextSectionName = mAttackMontage->GetSectionName(mAttackSectionIndex);
		Montage_JumpToSection(nextSectionName);
		return true;
	}

	if (!Montage_IsPlaying(mAttackMontage))
	{
		mAttackSectionIndex = 0;
		const FName nextSectionName = mAttackMontage->GetSectionName(mAttackSectionIndex);

		Montage_SetPosition(mAttackMontage, 0.0f);
		Montage_Play(mAttackMontage, PlayRate);
		Montage_JumpToSection(nextSectionName);
		return true;
	}

	return false;
}

UAnimSequence* UKGAnimInstance::FindAnimSequence(const FName Name) const
{
	if (!mAnimInfo)
		return nullptr;

	const TObjectPtr<UAnimSequence>* seq = mAnimInfo->SequenceMap.Find(Name);
	return nullptr != seq ? seq->Get() : nullptr;
}

UBlendSpace* UKGAnimInstance::FindBlendSpace(const FName Name) const
{
	if (!mAnimInfo)
		return nullptr;

	const TObjectPtr<UBlendSpace>* seq = mAnimInfo->BlendSpaceMap.Find(Name);
	return  nullptr != seq ? seq->Get() : nullptr;
}

UAnimMontage* UKGAnimInstance::FindAnimMontage(const FName Name) const
{
	if (!mAnimInfo)
		return nullptr;

	const TObjectPtr<UAnimMontage>* seq = mAnimInfo->MontageMap.Find(Name);
	return nullptr != seq ? seq->Get() : nullptr;
}

void UKGAnimInstance::PlayMontage(UAnimMontage* Montage, float PlayRate)
{
	if (!IsValid(Montage))
	{
		return;
	}
	Montage_SetPosition(Montage, 0.0f);
	Montage_Play(Montage, PlayRate);
}

void UKGAnimInstance::SetDir(float angle)
{	// -2 / 2 �� back
	mDir = angle / 90.0f;
}