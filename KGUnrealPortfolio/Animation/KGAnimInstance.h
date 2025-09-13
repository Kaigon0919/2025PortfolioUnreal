// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <KGCharacterType.h>
#include "DataTable/KGAnimInfo.h"
#include "KGAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillActiveDelegate, UAnimSequenceBase*);
DECLARE_MULTICAST_DELEGATE(FOnReadyComboDelegate);
/**
 * 
 */

UCLASS()
class KGUNREALPORTFOLIO_API UKGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UKGAnimInstance();
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	EKGCharacterType mCharacterType;

	const FKGAnimInfo* mAnimInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float mMoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool mIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool mIsGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float mLandRecoveryAlpha = 0.0f;

	bool mIsReadyCombo = false;
	bool mIsEnableReadyCombo = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage>	mAttackMontage;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage>	mLandRecoveryMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float mDir = 0.0f;

	int32		mAttackSectionIndex = 0;

public:
	virtual void PostInitProperties() override;
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
public:
	UFUNCTION()
	void AnimNotify_Attack();
	UFUNCTION()
	void AnimNotify_ReadyCombo();

	UFUNCTION()
	void AnimNotify_LandToLocoTransitionStart();
	UFUNCTION()
	void CustomAnimNotify_SkillActive(UAnimSequenceBase* Animation);

	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool Interrupted);
	bool PlayAttackMontage(float PlayRate);

public:
	UFUNCTION(BlueprintCallable, Meta = (BlueprintThreadSafe))
	UAnimSequence* FindAnimSequence(const FName Name) const;
	UFUNCTION(BlueprintCallable, Meta = (BlueprintThreadSafe))
	UBlendSpace* FindBlendSpace(const FName Name) const;
	UFUNCTION(BlueprintCallable, Meta = (BlueprintThreadSafe))
	UAnimMontage* FindAnimMontage(const FName Name) const;

	void PlayMontage(UAnimMontage* Montage, float PlayRate);

	void SetDir(float angle);

public:
	FOnSkillActiveDelegate OnSkillActiveDelegate;
	FOnReadyComboDelegate OnReadyComboDelegate;
};
