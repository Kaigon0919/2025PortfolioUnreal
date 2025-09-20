// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_KGOnSkillActive.h"
#include "KGAnimInstance.h"

void UAnimNotify_KGOnSkillActive::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (nullptr != MeshComp)
	{
		UKGAnimInstance* animInst = Cast<UKGAnimInstance>(MeshComp->GetAnimInstance());
		if (IsValid(animInst))
		{
			animInst->CuntomAnimNotify_SkillActive(Animation);
		}

	}

}