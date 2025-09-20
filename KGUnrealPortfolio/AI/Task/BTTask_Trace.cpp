// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Trace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

UBTTask_Trace::UBTTask_Trace()
{
	NodeName = TEXT("Trace Task");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	mTargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Trace, mTargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(Target);
	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (IsValid(AIController))
	{
		EPathFollowingStatus::Type result = AIController->GetMoveStatus();
		if (result == EPathFollowingStatus::Idle)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
	}

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BlackBoardComp->GetValueAsObject(mTargetKey.SelectedKeyName));
	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector OnwerLocation = AIController->GetPawn()->GetActorLocation();
	const float Distance = FVector::Dist2D(TargetLocation, OnwerLocation);
	const float AcceptanceRadius = BlackBoardComp->GetValueAsFloat(TEXT("AttackRange")) * 0.8f;

	if (Distance < AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}

EBTNodeResult::Type UBTTask_Trace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = EBTNodeResult::Aborted;
	return result;
}

void UBTTask_Trace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AIController->StopMovement();
	}
}
