// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed()
{
	if (APawn* PawnOwner = TryGetPawnOwner())
	{
		CurrentSpeed = static_cast<float>(PawnOwner->GetVelocity().Length());
	}
}