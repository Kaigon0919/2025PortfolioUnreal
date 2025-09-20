// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "KGAIController.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API AKGAIController : public AAIController
{
	GENERATED_BODY()
public:
	AKGAIController();
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAIPerceptionComponent>	mAIPerception;
	
	TObjectPtr<class UAISenseConfig_Sight>	mSightConfig;
public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void OnTarget(AActor* Target, FAIStimulus Stimulus);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	
};
