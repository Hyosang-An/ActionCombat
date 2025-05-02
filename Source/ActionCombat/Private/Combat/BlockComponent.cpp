// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BlockComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"

// Sets default values for this component's properties
UBlockComponent::UBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBlockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UBlockComponent::Check(AActor* Opponent)
{
	ACharacter* OwnerCharacter = GetOwner<ACharacter>();

	IMainPlayer* MainPlayerInterface = Cast<IMainPlayer>(OwnerCharacter);
	if (!MainPlayerInterface)
		return true;

	FVector OpponentLocation = Opponent->GetActorLocation();
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	FVector PlayerFowrard = OwnerCharacter->GetActorForwardVector();

	double Dot = FVector::DotProduct((OpponentLocation - OwnerLocation).GetSafeNormal(), PlayerFowrard.GetSafeNormal());
	if (Dot < FMath::Cos(FMath::DegreesToRadians(45.0)) || !MainPlayerInterface->HasEnoughStamina(StaminaCost))
		return true;

	OwnerCharacter->PlayAnimMontage(BlockAnimMontage);

	OnBlockDelegate.Broadcast(StaminaCost);
	return false;
}

