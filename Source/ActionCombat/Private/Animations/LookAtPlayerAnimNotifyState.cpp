// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/LookAtPlayerAnimNotifyState.h"

#include "Characters/LookAtPlayerComponent.h"

void ULookAtPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
		return;

	ULookAtPlayerComponent* LookAtComp = Owner->GetComponentByClass<ULookAtPlayerComponent>();
	if (!IsValid(LookAtComp))
		return;

	LookAtComp->bCanRotate = true;
}

void ULookAtPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner))
		return;

	ULookAtPlayerComponent* LookAtComp = Owner->GetComponentByClass<ULookAtPlayerComponent>();
	if (!IsValid(LookAtComp))
		return;

	LookAtComp->bCanRotate = false;
}