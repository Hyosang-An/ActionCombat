// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"

void UPlayerAnimInstance::UpdateSpeed()
{
	if (APawn* PawnOwner = TryGetPawnOwner())
	{
		CurrentSpeed = static_cast<float>(PawnOwner->GetVelocity().Length());
	}
}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActor)
{
	bIsInCombat = IsValid(NewTargetActor);
}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnOwner = TryGetPawnOwner();
	if (!IsValid(PawnOwner))
	{
		return;
	}

	if (!bIsInCombat)
	{
		return;
	}

	CurrentDirection =  UKismetAnimationLibrary::CalculateDirection(PawnOwner->GetVelocity(), PawnOwner->GetActorRotation());
}