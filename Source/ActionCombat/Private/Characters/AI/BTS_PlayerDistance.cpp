// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTS_PlayerDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_PlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FVector CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	float DistanceWithPlayer = static_cast<float>(FVector::Dist(CurrentLocation, PlayerLocation));

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("DistanceWithPlayer"), DistanceWithPlayer);
}