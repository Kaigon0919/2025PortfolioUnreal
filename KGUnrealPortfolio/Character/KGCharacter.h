// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KGCharacterType.h"
#include "InputAction.h"
#include "KGAlliance.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "GAS/Attributes/KGCharacterAttributeSet.h"

#include "KGCharacter.generated.h"

class UBehaviorTreeComponent;
class UKGBaseAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAiTaskAttackFinished, UBehaviorTreeComponent*, BehaviorTreeComp);

USTRUCT()
struct FSkillData
{
	GENERATED_BODY()

	UAnimMontage* Montage;
	FGameplayAbilitySpec AbilitySpec;


private:
	bool isValid = false;
public:
	void SetValid(bool value) { isValid = value; }
	bool IsValid() const { return isValid; }
};

UCLASS()
class KGUNREALPORTFOLIO_API AKGCharacter : public APawn, public IGenericTeamAgentInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
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
	EKGCharacterType mCharacterType;


public:
	EKGCharacterType GetCharacterType() const { return mCharacterType; }
#pragma region Input
private:
	enum EPlayerInputState
	{
		NONE,
		SKILL
	};
	EPlayerInputState mPlayerInputState;
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> mEnhancedInputSubSystem;
public:
	/*virtual*/ void MoveAction(const FInputActionValue& value);
	/*virtual*/ void RotateView(const FInputActionValue& value);
	/*virtual*/ void MouseRotateView(const FInputActionValue& value);
	/*virtual*/ void MouseDistanceAction(const FInputActionValue& value);
	/*virtual*/ void AttackKey();

	/*virtual*/ void Skill1Action(const FInputActionValue& value);
	/*virtual*/ void Skill2Action(const FInputActionValue& value);
	/*virtual*/ void Skill3Action(const FInputActionValue& value);
	/*virtual */void NormalAttackAction();
	/*virtual*/ void NormalAttack();

	void SetPlayerInputMode(EPlayerInputState value);
	bool IsInuptable()const;

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

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UKGCharacterMovement>	mMovement;

public:
	float GetCapsuleHalfHeight() const;

#pragma region AbilitySystem
protected:
	TObjectPtr<UKGCharacterAttributeSet> mAttributeSet;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> mAbilitySystemComponent;
public:
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void OnApplyGameplayEffectCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
#pragma endregion
#pragma region AI
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> mBehaviorTree;
public:
	FAiTaskAttackFinished mAiCharacterAttackFinished;
#pragma endregion

#pragma region Animation
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UKGAnimInstance> mAnimInst;
public:
	UKGAnimInstance* GetAnimInstance() const { return mAnimInst; }

private:
	UFUNCTION()
	virtual void OnReadyCombo();
	UFUNCTION()
	virtual void MontageEnd(UAnimMontage* Montage, bool Interrupted);

	void OnSkillActive(UAnimSequenceBase* Animation);
#pragma endregion

private:
	TArray<FSkillData> mSkillDataArray;
private:
	void SettingSkillAbility(FSkillData& skillData, TSubclassOf<UKGBaseAbility> baseAbility);
	void StartSkillAbility(FSkillData& skillData);
	void StartSkillMontage(FSkillData& skillData);
	bool CanActivateAbility(const FSkillData& skillData) const;
};
