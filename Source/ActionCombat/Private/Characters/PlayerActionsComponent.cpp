// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerActionsComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/MainPlayer.h"

// Sets default values for this component's properties
UPlayerActionsComponent::UPlayerActionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwner<ACharacter>();
	MovementComp = OwnerCharacter->GetCharacterMovement();

	MainPlayerInterface = Cast<IMainPlayer>(OwnerCharacter);
	check(MainPlayerInterface);
}


// Called every frame
void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Sprinting
	if (bIsSprinting)
	{
		// Sprint 불가하면 Walk로 전환됨
		Sprint();
	}
}

void UPlayerActionsComponent::Walk()
{
	MovementComp->MaxWalkSpeed = WalkSpeed;
	bIsSprinting = false;
}

void UPlayerActionsComponent::SprintStart()
{
	MovementComp->MaxWalkSpeed = SprintSpeed;
	bIsSprinting = true;
}

void UPlayerActionsComponent::Sprint()
{
	float SprintCost = GetWorld()->GetDeltaSeconds() * SprintCostPerSec;
	if (!MainPlayerInterface->HasEnoughStamina(SprintCost))
	{
		// 스태미나 0으로 만들기
		OnSprintedDelegate.Broadcast(SprintCost);
		Walk();
		return;
	}

	if (OwnerCharacter->GetVelocity().Equals(FVector::ZeroVector, 1))
		return;
	
	OnSprintedDelegate.Broadcast(SprintCost);
}

