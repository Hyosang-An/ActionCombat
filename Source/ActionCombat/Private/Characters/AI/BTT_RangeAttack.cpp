// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* OwnerCharacter = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();

	if (!IsValid(OwnerCharacter))
		return EBTNodeResult::Failed;

	OwnerCharacter->PlayAnimMontage(AnimMontage);

	double RandomValue = FMath::RandRange(0.0, 1.0);
	
	if (RandomValue > Threshold)
	{
		Threshold = 0.9;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Charge);
	}
	else
	{
		Threshold -= 0.1;
	}

	return EBTNodeResult::Succeeded;
}