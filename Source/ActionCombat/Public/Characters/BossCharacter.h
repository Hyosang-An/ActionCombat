// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.h"
#include "GameFramework/Character.h"
#include "Interfaces/Enemy.h"
#include "Interfaces/Fighter.h"
#include "BossCharacter.generated.h"

UCLASS()
class ACTIONCOMBAT_API ABossCharacter : public ACharacter, public IEnemy, public IFighter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState;

	UPROPERTY(VisibleAnywhere)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeathAnimMontage;

	UPROPERTY()
	TObjectPtr<class AAIController> AIController;

public:
	// Sets default values for this character's properties
	ABossCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStatsComponent> StatsComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCombatComponent> CombatComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	virtual float GetDamage() override;
	virtual void Attack() override;

	virtual float GetAnimationDuration() override;
	virtual float GetMeleeRange() override;

	UFUNCTION()
	void HandlePlayerDeath();

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	UFUNCTION()
	void FinishDeathAnim();
};