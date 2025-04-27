// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EStat.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount);

	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();

	UFUNCTION(BlueprintCallable)
	void EnableRegenStamina();

private:
	// 타이머 핸들
	FTimerHandle RegenStaminaTimerHandle;
};