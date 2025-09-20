// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KGAIController.h"
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Navigation/PathFollowingComponent.h>
#include <Components/KGCharacterMovement.h>

AKGAIController::AKGAIController()
{
	mAIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*mAIPerception);

	{ // 임시로 하드 코딩. 추후 몬스터마다 별도의 값을 갖도록 수정필요.
		mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
		// 시야 반경
		mSightConfig->SightRadius = 1000.f;
		// 상실 거리. 
		mSightConfig->LoseSightRadius = mSightConfig->SightRadius + 200.f;
		// 시야 각도. 반각이다. 정면 기준으로의 시야 각도를 지정.
		mSightConfig->PeripheralVisionAngleDegrees = 180.f;
		// AI가 마지막으로 본 위치와 가까우면 감지 성공하게 하는 거리.
		mSightConfig->AutoSuccessRangeFromLastSeenLocation = 200.f;
		// 감지 위치를 뒤로 오프셋 하기 위한 값.
		mSightConfig->PointOfViewBackwardOffset = 0.f;
		// AI와 너무 가까운 곳은 감지 안 하도록 필터링.
		mSightConfig->NearClippingRadius = 0.f;
		// 적, 중립, 아군에 대한 감지를 할 것인지 여부를 지정.
		mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
		mSightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		mAIPerception->ConfigureSense(*mSightConfig);
		mAIPerception->SetDominantSense(mSightConfig->GetSenseImplementation());
	}

	// TeamId는 해당 컨트롤러가 갖고있는 캐릭터가 직접 외부에서 호출해 등록해주는것으로 처리한다.
	//SetGenericTeamId(FGenericTeamId());

	mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this,&AKGAIController::OnTarget);
}

ETeamAttitude::Type AKGAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	// 이 클래스를 상속받는 객체가 아닐 경우
	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;
	
	// 일단 같은 Id가 아니면 다 적이다.
	if (OtherTeamAgent->GetGenericTeamId().GetId() == GetGenericTeamId())
		return ETeamAttitude::Friendly;

	return ETeamAttitude::Hostile;

}

void AKGAIController::OnTarget(AActor* Target, FAIStimulus Stimulus)
{
	TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

	if (!IsValid(Blackboard))
	{
		return;
	}

	Blackboard->SetValueAsObject(TEXT("TargetActor"), Stimulus.WasSuccessfullySensed() ? Target : nullptr);
}

void AKGAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AAIController::OnMoveCompleted(RequestID, Result);
	// 경로 이동이 성공했을 경우나 중단되었을 경우(Abort) 등등.
	if (Result.Code == EPathFollowingResult::Success)
	{
		// 이 컨트롤러가 빙의된 폰을 얻어온다.
		APawn* ControlPawn = GetPawn();

		if (ControlPawn)
		{
			UKGCharacterMovement* MoveComp = Cast<UKGCharacterMovement>(ControlPawn->GetMovementComponent());

			if (MoveComp)
			{
				MoveComp->StopMovementImmediately();
			}
		}
	}
}
