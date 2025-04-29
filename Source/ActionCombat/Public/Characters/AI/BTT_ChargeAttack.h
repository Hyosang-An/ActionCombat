// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ChargeAttack();

private:
	class AAIController*     AIController;
	class ACharacter*        OwnerCharacter;
	class UBossAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.f };

	float OriginalWalkSpeed;

	UPROPERTY(EditAnywhere)
	float ChargeWalkSpeed{ 2000.f };

	bool bIsFinished{ false };

	FScriptDelegate MoveCompletedDelegate;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void ChargeAtPlayer();

	UFUNCTION()
	void HandleMoveCompleted();

	void FinishAttackTask();
};