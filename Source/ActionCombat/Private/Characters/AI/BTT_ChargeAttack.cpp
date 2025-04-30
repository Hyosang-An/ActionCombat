// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_ChargeAttack.h"

#include "AIController.h"
#include "Animations/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	// 이 매크로 하나로 bNotifyTick, bCreateNodeInstance, bNotifyTaskFinished 등이 설정됩니다.
	INIT_TASK_NODE_NOTIFY_FLAGS();

	MoveCompletedDelegate.BindUFunction(this, "HandleMoveCompleted");
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReadyToCharge = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsReadyToCharge"));

	// AnimNotify에서 변경됨
	if (bIsReadyToCharge)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

		ChargeAtPlayer();
	}

	if (!bIsFinished)
		return;

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
	
	AIController->ReceiveMoveCompleted.Remove(MoveCompletedDelegate);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();
	OwnerCharacter = AIController->GetCharacter();
	AnimInstance = Cast<UBossAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());

	// AnimInstance에서 State Machine 상태 전이를 위한 bool변수 변경
	AnimInstance->bIsCharging = true;

	// BT에서 행동상태 추적을 위한 Blackboard bool변수 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

	bIsFinished = false;

	return EBTNodeResult::InProgress;
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector        PlayerLocation = Player->GetActorLocation();
	FAIMoveRequest MoveRequest = FAIMoveRequest{ PlayerLocation };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);

	FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest);
	if (Result == EPathFollowingRequestResult::AlreadyAtGoal || Result == EPathFollowingRequestResult::Failed)
	{
		HandleMoveCompleted();
		return;
	}
	
	AIController->SetFocus(Player);

	// 계속 쫒아오게 만들고 싶은 경우
	//AIController->MoveToActor(Player, 100);

	AIController->ReceiveMoveCompleted.AddUnique(MoveCompletedDelegate);

	OriginalWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = ChargeWalkSpeed;
}

void UBTT_ChargeAttack::HandleMoveCompleted()
{
	AnimInstance->bIsCharging = false;

	FTimerHandle AttackTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UBTT_ChargeAttack::FinishAttackTask, 1);

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	bIsFinished = true;
}