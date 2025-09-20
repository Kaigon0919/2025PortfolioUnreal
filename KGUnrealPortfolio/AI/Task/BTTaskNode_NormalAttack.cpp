// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_NormalAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Character/KGCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/KGAnimInstance.h"


UBTTaskNode_NormalAttack::UBTTaskNode_NormalAttack()
{
	NodeName = TEXT("Attack Task");
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	mTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_NormalAttack, mTargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackBoardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BlackBoardComp->GetValueAsObject(mTargetKey.SelectedKeyName));

	if (!Target)
	{
		return EBTNodeResult::Succeeded;
	}

	BlackBoardComp->SetValueAsObject(TEXT("AttackTarget"), Target);

	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = 0.0f;
	FVector OnwerLocation = AIController->GetPawn()->GetActorLocation();
	OnwerLocation.Z = 0.0f;

	AKGCharacter* Character= Cast<AKGCharacter>(AIController->GetPawn());
	if (Character)
	{
		Character->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OnwerLocation, TargetLocation));
		Character->NormalAttackAction();
		AddEvent(Character);
	}
	else
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_NormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* Pawn = AIController->GetPawn();

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BlackBoardComp->GetValueAsObject(mTargetKey.SelectedKeyName));
	if (!Target)
	{
		if (!IsValid(Pawn))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
			return;
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AKGCharacter* Character = Cast<AKGCharacter>(Pawn);
	if (Character)
	{
		const UKGAnimInstance* AnimInst = Character->GetAnimInstance();
		if (AnimInst && !AnimInst->IsAnyMontagePlaying())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = EBTNodeResult::Aborted;
	AKGCharacter* Character = Cast<AKGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character)
	{
		RemoveEvent(Character);
	}

	return result;
}

void UBTTaskNode_NormalAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();

	BlackBoardComp->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	AKGCharacter* Character = Cast<AKGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character)
	{
		RemoveEvent(Character);
	}
}

void UBTTaskNode_NormalAttack::EndAttackAnimation(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTaskNode_NormalAttack::AddEvent(AKGCharacter* character)
{
	check(character);
	character->mAiCharacterAttackFinished.AddDynamic(this, &UBTTaskNode_NormalAttack::EndAttackAnimation);
}

void UBTTaskNode_NormalAttack::RemoveEvent(AKGCharacter* character)
{
	check(character);
	character->mAiCharacterAttackFinished.RemoveDynamic(this, &UBTTaskNode_NormalAttack::EndAttackAnimation);
}
