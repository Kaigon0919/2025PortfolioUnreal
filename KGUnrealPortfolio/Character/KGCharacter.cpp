// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KGCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Input/KGInGameInput.h>

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/KGCharacterMovement.h"
#include "Camera/CameraComponent.h"
#include "Animation/KGAnimInstance.h"
#include "KGAssetManager.h"
#include <AbilitySystemComponent.h>

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "KGUtil.h"
#include <DataTable/KGCharacterStatusInfo.h>

#include <Controller/KGAIController.h>
#include <Controller/KGPlayerController.h>

#include "GAS/Ability/KGNormalAttackAbility.h"
#include <GAS/Ability/KGHPRegenerationAbility.h>
#include <GAS/Ability/KGMPRegenerationAbility.h>
#include <GAS/Ability/KGSPRegenerationAbility.h>

#define SKILL_MAX_NUMBER 3

// Sets default values
AKGCharacter::AKGCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		mCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
		mCapsuleComponent->bVisualizeComponent = true;
		mCapsuleComponent->SetCollisionProfileName(TEXT("KGCharacter"));
		mCapsuleComponent->SetCanEverAffectNavigation(false);

		SetRootComponent(mCapsuleComponent);
	}

	{
		mMesh = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("Mesh"));
		mMesh->SetupAttachment(mCapsuleComponent);
		mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mMesh->SetCanEverAffectNavigation(false);
		mMesh->bRenderCustomDepth = true;
	}

	{
		mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		mSpringArm->SetupAttachment(mCapsuleComponent);
		mSpringArm->TargetArmLength = 200.0f;
		mSpringArm->bUsePawnControlRotation = true;
	}

	{
		mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		mCamera->SetupAttachment(mSpringArm);
		mCamera->bUsePawnControlRotation = false;

		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = true;
	}

	{
		mMovement = CreateDefaultSubobject<UKGCharacterMovement>(TEXT("Movement"));
		mMovement->SetUpdatedComponent(GetRootComponent());
	}


	{
		mAttributeSet = CreateDefaultSubobject<UKGCharacterAttributeSet>(TEXT("AttributeSet"));
		mAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
		mAbilitySystemComponent->AddAttributeSetSubobject<UKGCharacterAttributeSet>(mAttributeSet);
	}

	{
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		AIControllerClass = AKGAIController::StaticClass();
	}
}

void AKGCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (mCharacterType != EKGCharacterType::None)
	{
		FKGAnimInfo* AnimInfo = UKGAssetManager::Get().FindDataTableRow<FKGAnimInfo>(TEXT("DTCharacterAnimInfo"), mCharacterType);
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
	UE_LOG(KGLog, Log, TEXT("Name : %s / Function : %s"), *GetName(), TEXT(__FUNCTION__));
}

void AKGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(KGLog, Log, TEXT("Name : %s / Function : %s"), *GetName(), TEXT(__FUNCTION__));
}

// Called when the game starts or when spawned
void AKGCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

	mAnimInst = Cast<UKGAnimInstance>(mMesh->GetAnimInstance());
	mAnimInst->OnMontageEnded.AddDynamic(this, &AKGCharacter::MontageEnd);
	mAnimInst->OnReadyComboDelegate.AddDynamic(this, &AKGCharacter::OnReadyCombo);
	mAnimInst->OnSkillActiveDelegate.AddUObject(this, &AKGCharacter::OnSkillActive);

	FKGCharacterStatusInfo* Info = UKGAssetManager::Get().FindDataTableRow<FKGCharacterStatusInfo>(TEXT("DTCharacterStatusInfo"), mCharacterType);
	if (Info)
	{
		mAttributeSet->InitAttack(Info->Attack);
		mAttributeSet->InitAttackSpeed(Info->AttackSpeed);
		mAttributeSet->InitDefence(Info->Defence);

		mAttributeSet->InitHP(Info->HP);
		mAttributeSet->InitHPMax(Info->HP);
		mAttributeSet->InitHPRecovery(Info->HPRecovery);

		mAttributeSet->InitMP(Info->MP);
		mAttributeSet->InitMPMax(Info->MP);
		mAttributeSet->InitMPRecovery(Info->MPRecovery);

		mAttributeSet->InitSP(Info->SP);
		mAttributeSet->InitSPMax(Info->SP);
		mAttributeSet->InitSPRecovery(Info->SPRecovery);

		mAttributeSet->InitLevel(Info->Level);
		mAttributeSet->InitExp(Info->Exp);
		mAttributeSet->InitGold(Info->Gold);
	}

	AKGAIController* AIController = GetController<AKGAIController>();
	if (AIController && mBehaviorTree)
	{
		AIController->SetGenericTeamId(GetGenericTeamId());
		AIController->RunBehaviorTree(mBehaviorTree);
		// 공격거리는 추후 일반 공격 어빌리티를 만들고 일반 공격 어빌리티의 거리를 통해 세팅되도록한다.
		//AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), mMonsterData.AttackRange);
	}

	{
		mAbilitySystemComponent->InitAbilityActorInfo(this, this);
		mAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UKGNormalAttackAbility::StaticClass()));

		// RegenerationAbility
		{
			FGameplayAbilitySpecHandle  HPRegenerationAbilityHandle = mAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UKGHPRegenerationAbility::StaticClass()));
			mAbilitySystemComponent->TryActivateAbility(HPRegenerationAbilityHandle);
			FGameplayAbilitySpecHandle  MPRegenerationAbilityHandle = mAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UKGMPRegenerationAbility::StaticClass()));
			mAbilitySystemComponent->TryActivateAbility(MPRegenerationAbilityHandle);
			FGameplayAbilitySpecHandle  SPRegenerationAbilityHandle = mAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UKGSPRegenerationAbility::StaticClass()));
			mAbilitySystemComponent->TryActivateAbility(SPRegenerationAbilityHandle);
		}

		mSkillDataArray.SetNum(SKILL_MAX_NUMBER);

		FKGAnimInfo* AnimInfo = UKGAssetManager::Get().FindDataTableRow<FKGAnimInfo>(TEXT("DTCharacterAnimInfo"), mCharacterType);
		if (nullptr != AnimInfo)
		{			
			// Note 25.09.20 패시브등 애니메이션이 존재 하지 않을 수도 있다. nullptr 허용. 해당 변수 사용시 확인 필수.
			TObjectPtr<UAnimMontage>* Montage = AnimInfo->MontageMap.Find(TEXT("Skill1"));
			mSkillDataArray[0].Montage = (nullptr != Montage ? Montage->Get() : nullptr);
			Montage = AnimInfo->MontageMap.Find(TEXT("Skill2"));
			mSkillDataArray[1].Montage = (nullptr != Montage ? Montage->Get() : nullptr);
			Montage = AnimInfo->MontageMap.Find(TEXT("Skill3"));
			mSkillDataArray[2].Montage = (nullptr != Montage ? Montage->Get() : nullptr);
		}
		SettingSkillAbility(mSkillDataArray[0], Info->SkillAbility1);
		SettingSkillAbility(mSkillDataArray[1], Info->SkillAbility2);
		SettingSkillAbility(mSkillDataArray[2], Info->SkillAbility3);
	}
}

// Called every frame
void AKGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPlayerControlled())
	{
		FString DebugMessage;
		mAttributeSet->GetDebugInfoString(DebugMessage);
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, DebugMessage);
	}
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
			inputComp->BindAction(CDOGameInput->GetInputMouseViewDistanceAction(), ETriggerEvent::Triggered, this, &AKGCharacter::MouseDistanceAction);

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

void AKGCharacter::MouseDistanceAction(const FInputActionValue& value)
{
	float actionValue = value.Get<float>();
	const float moveSpeedRatio = 5.f;
	float result = FMath::Clamp(mSpringArm->TargetArmLength - (actionValue * moveSpeedRatio), 100, 500);
	mSpringArm->TargetArmLength = result;
}

void AKGCharacter::AttackKey()
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));

	NormalAttackAction();
}

void AKGCharacter::Skill1Action(const FInputActionValue& value)
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
	StartSkillMontage(mSkillDataArray[0]);
}

void AKGCharacter::Skill2Action(const FInputActionValue& value)
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
	StartSkillMontage(mSkillDataArray[1]);
}

void AKGCharacter::Skill3Action(const FInputActionValue& value)
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
	StartSkillMontage(mSkillDataArray[2]);
}

void AKGCharacter::NormalAttackAction()
{
	UE_LOG(KGLog, Log, TEXT("%s"), TEXT(__FUNCTION__));
	if (!mAbilitySystemComponent)
	{
		UE_LOG(KGLog, Warning, TEXT("falied, mAbilitySystemComponent is nullptr"));
		return;
	}

	FGameplayAbilitySpec* Spec = mAbilitySystemComponent->FindAbilitySpecFromClass(UKGNormalAttackAbility::StaticClass());
	if (!Spec)
	{
		UE_LOG(KGLog, Warning, TEXT("falied, Spec(UKGNormalAttackAbility) is nullptr"));
		return;
	}

	UGameplayAbility* Ability = Spec->Ability;
	const FGameplayAbilityActorInfo* ActorInfo = mAbilitySystemComponent->AbilityActorInfo.Get();
	if (false == Ability->CanActivateAbility(Spec->Handle, ActorInfo))
	{
		return;
	}

	if (IsValid(mAnimInst))
	{
		bool succeed = mAnimInst->PlayAttackMontage(1.f);
	}
}

void AKGCharacter::NormalAttack()
{
	mAbilitySystemComponent->TryActivateAbilityByClass(UKGNormalAttackAbility::StaticClass());
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

UAbilitySystemComponent* AKGCharacter::GetAbilitySystemComponent() const
{
	return mAbilitySystemComponent;
}

void AKGCharacter::OnReadyCombo()
{
	if (!IsPlayerControlled())
	{
		AKGAIController* aiController = GetController<AKGAIController>();
		UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(aiController->GetBrainComponent());
		mAiCharacterAttackFinished.Broadcast(BTComp);
	}
}

void AKGCharacter::MontageEnd(UAnimMontage* Montage, bool Interrupted)
{
	UAnimMontage* NormalAttackMontage = mAnimInst->FindAnimMontage(TEXT("NormalAttack"));
	if (nullptr != NormalAttackMontage && Montage == NormalAttackMontage)
	{
		if (!IsPlayerControlled())
		{
			AKGAIController* aiController = GetController<AKGAIController>();
			UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(aiController->GetBrainComponent());
			mAiCharacterAttackFinished.Broadcast(BTComp);
		}

		return;
	}

	for (auto data : mSkillDataArray)
	{
		if (data.Montage != Montage)
		{
			continue;
		}

		SetPlayerInputMode(EPlayerInputState::NONE);
		return;
	}
}

void AKGCharacter::OnSkillActive(UAnimSequenceBase* Animation)
{
	UAnimMontage* Montage = Cast<UAnimMontage>(Animation);
	if (IsValid(Montage))
	{
		for (auto data : mSkillDataArray)
		{
			if (data.Montage != Montage)
			{
				continue;
			}

			StartSkillAbility(data);
			break;
		}
	}
}

void AKGCharacter::SettingSkillAbility(FSkillData& skillData, TSubclassOf<UKGBaseAbility> baseAbility)
{
	if (!IsValid(baseAbility))
	{
		skillData.SetValid(false);
		return;
	}

	const UKGBaseAbility* DefaultObj = baseAbility->GetDefaultObject<UKGBaseAbility>();
	if (!DefaultObj)
	{
		skillData.SetValid(false);
		return;
	}

	skillData.AbilitySpec = FGameplayAbilitySpec(baseAbility);
	FGameplayAbilitySpecHandle handle = mAbilitySystemComponent->GiveAbility(skillData.AbilitySpec);
	skillData.SetValid(handle.IsValid());
	if (false == skillData.IsValid())
	{
		return;
	}

	switch (DefaultObj->GetSkillType())
	{
	case EKGSkillType::Active:
		if(!skillData.Montage)
		{
			skillData.SetValid(false);
			check(false);
			return;
		}
		break;
	case EKGSkillType::Buff:
		{
			__noop;
		}
		break;
	case EKGSkillType::Passive:
		{
			mAbilitySystemComponent->TryActivateAbility(handle);
		}
		break;
	}

}

void AKGCharacter::StartSkillAbility(FSkillData& skillData)
{
	if (!skillData.IsValid())
	{
		return;
	}

	const UKGBaseAbility* ability = Cast<UKGBaseAbility>(skillData.AbilitySpec.Ability.Get());
	switch (ability->GetSkillType())
	{
	case EKGSkillType::Passive:
		{
			return;
		}
		break;
	}

	mAbilitySystemComponent->TryActivateAbility(skillData.AbilitySpec.Handle);
}

void AKGCharacter::StartSkillMontage(FSkillData& skillData)
{
	if (!IsValid(skillData.Montage))
	{
		return;
	}

	float PlayRate = 1.0f;
	mAnimInst->PlayMontage(skillData.Montage, PlayRate);
	//float Duration = skillData.Montage->GetPlayLength() / PlayRate;

	SetPlayerInputMode(EPlayerInputState::SKILL);

}

void AKGCharacter::SetPlayerInputMode(EPlayerInputState value)
{
	if (value == mPlayerInputState)
	{
		return;
	}

	const UKGInGameInput* CDOPlatformInput = GetDefault<UKGInGameInput>();

	// 이전 값에 따른 복구처리.
	switch (mPlayerInputState)
	{
	case EPlayerInputState::NONE:
		break;
	case EPlayerInputState::SKILL:
		mEnhancedInputSubSystem->AddMappingContext(CDOPlatformInput->GetInputContext(), 0);
		break;
	default:
		break;
	}

	switch (value)
	{
	case EPlayerInputState::NONE:
		break;
	case EPlayerInputState::SKILL:
		mEnhancedInputSubSystem->RemoveMappingContext(CDOPlatformInput->GetInputContext());
		break;
	default:
		break;
	}

	mPlayerInputState = value;
}

bool AKGCharacter::IsInuptable() const
{
	if (mPlayerInputState == EPlayerInputState::SKILL)
	{
		return false;
	}

	return true;
}
