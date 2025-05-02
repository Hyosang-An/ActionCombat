// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EStat.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthPercentageUpdateSignature, float, HealthPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaPercentageUpdateSignature, float, StaminaPercentage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroHealthSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	double StaminaRegenRate{ 10 };

	UPROPERTY(VisibleAnywhere)
	bool bCanRegenStamina{ true };

	UPROPERTY(EditAnywhere)
	float StaminaDelayDuration{ 2.f };

public:
	// Sets default values for this component's properties
	UStatsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStat, float> Stats;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthPercentageUpdateSignature OnHealthPercentageUpdateDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStaminaPercentageUpdateSignature OnStaminaPercentageUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthSignature OnZeroHealthDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount, AActor* Opponent);

	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();

	UFUNCTION(BlueprintCallable)
	void EnableRegenStamina();

	UFUNCTION(BlueprintPure) // BlueprintPure는 "실행 핀이 없는 함수"로, 멤버변수를 읽기만 가능하고 쓰기는 불가.
	float GetStatPercentage(EStat Current, EStat Max);

private:
	// 타이머 핸들
	FTimerHandle RegenStaminaTimerHandle;
};