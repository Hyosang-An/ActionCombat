// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"

#include "AIController.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* OwnerCharacter = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();

	if (!IsValid(OwnerCharacter))
		return EBTNodeResult::Failed;

	OwnerCharacter->PlayAnimMontage(AnimMontage);
	
	return EBTNodeResult::Succeeded;

	
}