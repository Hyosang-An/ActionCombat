// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"

#include <rapidjson/rapidjson.h>

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::ComboAttack()
{
	if (IMainPlayer* MainPlayerInterface = Cast<IMainPlayer>(OwnerCharacter); MainPlayerInterface && !MainPlayerInterface->HasEnoughStamina(StaminaCost))
	{
		return;
	}
	
	if (!bCanAttack)
	{
		return;
	}
	bCanAttack = false;

	if (AttackAnimations.IsEmpty())
	{
		return;
	}
	OwnerCharacter->PlayAnimMontage(AttackAnimations[ComboCount]);

	ComboCount++;

	int32 MaxComboCount{ AttackAnimations.Num() };
	if (ComboCount >= MaxComboCount)
	{
		ComboCount = 0;
	}

	OnAttackPerformedDelegate.Broadcast(StaminaCost);
}

void UCombatComponent::HandleResetAttack()
{
	bCanAttack = true;
}

void UCombatComponent::RandomAttack()
{
	int32 RandomIndex = FMath::RandRange(0, AttackAnimations.Num() - 1);

	AnimationDuration = OwnerCharacter->PlayAnimMontage(AttackAnimations[RandomIndex]);
}