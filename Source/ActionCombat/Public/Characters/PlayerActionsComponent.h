// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprintedSignature, float, Amount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRollSignature, float, Amount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class ACharacter> OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComp;

	class IMainPlayer* MainPlayerInterface;

	UPROPERTY(EditAnywhere)
	float SprintCostPerSec{ 10.f };

	UPROPERTY(EditAnywhere)
	float SprintSpeed{ 1000.f };

	UPROPERTY(EditAnywhere)
	float WalkSpeed{ 500.f };

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> RollAnimMontage;

	UPROPERTY(EditAnywhere)
	float RollCost{ 5.f };


public:
	// Sets default values for this component's properties
	UPlayerActionsComponent();

	UPROPERTY(BlueprintAssignable)
	FOnSprintedSignature OnSprintedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnRollSignature OnRollDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void SprintStart();

	UFUNCTION(BlueprintCallable)
	void Sprint();

	bool bIsSprinting{ false };

	UFUNCTION(BlueprintCallable)
	void Roll();
	
	bool bIsRolling{ false };

	
};