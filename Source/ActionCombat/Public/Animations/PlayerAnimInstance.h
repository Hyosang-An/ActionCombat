// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSpeed{ 0.f };

	UFUNCTION(blueprintCallable)
	void UpdateSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInCombat{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentDirection{ 0.f };

public:
	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActor);

	UFUNCTION(BlueprintCallable)
	void UpdateDirection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlocking{ false };
};