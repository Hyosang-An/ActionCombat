// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;

	UPROPERTY(EditAnywhere)
	FName RotationSocketName;

	UPROPERTY(EditAnywhere)
	double BoxCollisionLength = 30;

	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	bool bDebugMode = false;

	TArray<TObjectPtr<AActor>> TargetsToIgnore;

public:
	// Sets default values for this component's properties
	UTraceComponent();

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking{ false };
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();
};