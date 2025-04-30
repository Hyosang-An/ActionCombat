// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_MeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	// 이 매크로 하나로 bNotifyTick, bCreateNodeInstance, bNotifyTaskFinished 등이 설정됩니다.
	INIT_TASK_NODE_NOTIFY_FLAGS();

	MoveDelegate.BindUFunction(this, "FinishAttackTask");
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bIsFinished = false;

	float DistanceWithPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceWithPlayer"));

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (DistanceWithPlayer > AttackRadius)
	{

		APawn*                            Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		EPathFollowingRequestResult::Type Result = AIController->MoveToActor(Player, AcceptableRadius, false);
		if (Result != EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			// 바로 종료
			FinishAttackTask();
		}
		AIController->ReceiveMoveCompleted.AddUnique(MoveDelegate);
	}
	else
	{
		IFighter* Fighter = Cast<IFighter>(AIController->GetPawn());
		if (Fighter)
		{
			Fighter->Attack();

			FTimerHandle AttackTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UBTT_MeleeAttack::FinishAttackTask, Fighter->GetAnimationDuration());
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	float DistanceWithPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DistanceWithPlayer"));
	AAIController* AIController = OwnerComp.GetAIOwner();

	IFighter* Fighter = Cast<IFighter>(AIController->GetPawn());
	check(Fighter);

	if (DistanceWithPlayer > Fighter->GetMeleeRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);

		// BT에서 Obeserve Aborts로 abort됨
		// AbortTask(OwnerComp, NodeMemory);

		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		AIController->StopMovement();
		AIController->ClearFocus(EAIFocusPriority::Gameplay);

		AIController->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsFinished)
		return;

	OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_MeleeAttack::FinishAttackTask()
{
	bIsFinished = true;
}