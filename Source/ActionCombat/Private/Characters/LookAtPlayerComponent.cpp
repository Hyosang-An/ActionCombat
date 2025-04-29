// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LookAtPlayerComponent.h"

#include "EditorCategoryUtils.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULookAtPlayerComponent::ULookAtPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void ULookAtPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!bCanRotate)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	APawn*  Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector OwnerLocation = Owner->GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();

	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, PlayerLocation);
	FRotator CurrentRotation = Owner->GetActorRotation();
	FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(CurrentRotation, DesiredRotation, DeltaTime, RotationSpeed);

	FRotator NewYawOnlyRotator = FRotator(CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll);
	Owner->SetActorRotation(NewYawOnlyRotator);
}