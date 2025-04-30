// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* OwnerCharacter = OwnerComp.GetAIOwner()->GetPawn<ACharacter>();

	if (!IsValid(OwnerCharacter))
		return EBTNodeResult::Failed;

	float DistanceWithPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceWithPlayer"));
	IFighter* Fighter = Cast<IFighter>(OwnerCharacter);
	check(Fighter);
	if (DistanceWithPlayer < Fighter->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);

		// AbortTask직접 호출하지 않아도 현재 구조상 상위 데코레이터가 Observer aborts: Self 로 abort 시켜버림
		// AbortTask(OwnerComp, NodeMemory);
		return EBTNodeResult::Aborted;
	}

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