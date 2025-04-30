// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MeleeAttack();

private:
	UPROPERTY(EditAnywhere)
	float AttackRadius = 200.f;

	UPROPERTY(EditAnywhere)
	float AcceptableRadius = 150.f;

	FScriptDelegate MoveDelegate;

	bool bIsFinished = false;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void FinishAttackTask();
};