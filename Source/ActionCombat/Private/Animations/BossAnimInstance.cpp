// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BossAnimInstance.h"

void UBossAnimInstance::UpdateSpeed()
{
	if (APawn* PawnOwner = TryGetPawnOwner())
	{
		CurrentSpeed = static_cast<float>(PawnOwner->GetVelocity().Length());
	}
}