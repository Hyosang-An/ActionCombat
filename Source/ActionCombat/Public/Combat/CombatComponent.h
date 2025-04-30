// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackPerformedSignature, float, Amount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UAnimMontage>> AttackAnimations;

	UPROPERTY()
	TObjectPtr<class ACharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere)
	int32 ComboCount{ 0 };

	UPROPERTY(VisibleAnywhere)
	bool bCanAttack{ true };

	UPROPERTY(EditAnywhere)
	float StaminaCost{ 5 };

	float AnimationDuration{ 0.f };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION(blueprintCallable)
	void HandleResetAttack();

	UFUNCTION(BlueprintCallable)
	void RandomAttack();
	
};