// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "KGBaseCueNotify_Static.generated.h"

/**
 * 
 */
UCLASS()
class KGUNREALPORTFOLIO_API UKGBaseCueNotify_Static : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UKGBaseCueNotify_Static();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem>	mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem>	mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase>	mSound;
};
