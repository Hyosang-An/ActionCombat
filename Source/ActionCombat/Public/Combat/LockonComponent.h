// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"

class ULockonComponent;
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnUpdatedTargetSignature, ULockonComponent, OnUpdatedTargetDelegate, AActor*, NewTargetActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONCOMBAT_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;
	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
	UPROPERTY()
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

public:
	// Sets default values for this component's properties
	ULockonComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> CurrentTargetActor;

	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature OnUpdatedTargetDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartLockon(float Radius = 750.f);

	UFUNCTION(BlueprintCallable)
	void ToggleLockon(float Radius = 750.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double BreakLockonDistance{ 1000 };

public:
	UFUNCTION(BlueprintCallable)
	void StopLockon();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};