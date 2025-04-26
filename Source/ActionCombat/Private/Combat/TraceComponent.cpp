// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"

#include "Interfaces/Fighter.h"
#include "Engine/DamageEvents.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking)
		return;

	FVector StartSocketLocation = SkeletalMeshComponent->GetSocketLocation(TraceStartSocketName);
	FVector EndSocketLocation = SkeletalMeshComponent->GetSocketLocation(TraceEndSocketName);
	FQuat   ShapeRotation = SkeletalMeshComponent->GetSocketQuaternion(RotationSocketName);

	TArray<FHitResult> HitResults;
	double             WeaponDistance = FVector::Dist(StartSocketLocation, EndSocketLocation);
	FVector            BoxHalfExtent{ BoxCollisionLength, BoxCollisionLength, WeaponDistance };
	BoxHalfExtent /= 2.0f;
	FCollisionShape       Box = FCollisionShape::MakeBox(BoxHalfExtent);
	FVector               CenterLocation = (StartSocketLocation + EndSocketLocation) * 0.5;
	FCollisionQueryParams IgnoreParams{ TEXT("Ignore Params"), false, GetOwner() }; // 추가로 무시할게 있다면 IgnoreParams.AddIgnoredActor 또는 IgnoreParams.AddIgnoredActors 호출
	bool                  bHasResult = GetWorld()->SweepMultiByChannel(HitResults, CenterLocation, CenterLocation, ShapeRotation, ECC_GameTraceChannel1, Box, IgnoreParams);

	if (bDebugMode)
	{
		DrawDebugBox(GetWorld(), CenterLocation, Box.GetExtent(), ShapeRotation, bHasResult ? FColor::Green : FColor::Red, false, 0.3);
	}

	if (HitResults.IsEmpty())
		return;

	float     CharacterDamage = 0;
	IFighter* Fighter = Cast<IFighter>(GetOwner());
	if (Fighter)
	{
		CharacterDamage = Fighter->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;
	for (const auto& HitResult : HitResults)
	{
		AActor* TargetActor = HitResult.GetActor();

		if (TargetsToIgnore.Contains(TargetActor))
			continue;

		TargetActor->TakeDamage(CharacterDamage, TargetAttackedEvent, GetOwner()->GetInstigatorController(), GetOwner());

		TargetsToIgnore.AddUnique(TargetActor);
	}

	// ==============================================================================================
	// OverlapMultiByChannel 사용하는 법

	// // 1) 칼끝과 손잡이 소켓 위치
	// FVector StartSocketLocation = SkeletalMeshComponent->GetSocketLocation(TraceStartSocketName);
	// FVector EndSocketLocation = SkeletalMeshComponent->GetSocketLocation(TraceEndSocketName);
	//
	// // 2) 칼날 길이와 센터 계산
	// double  WeaponDistance = FVector::Dist(StartSocketLocation, EndSocketLocation);
	// FVector CenterLocation = (StartSocketLocation + EndSocketLocation) * 0.5;
	//
	// // 3) 박스 반지름(절반 크기) :  X·Y = 칼 두께/2,  Z = 칼 길이/2
	// FVector BoxHalfExtent{ BoxCollisionLength, BoxCollisionLength, WeaponDistance * 0.5 };
	//
	// // 4) 충돌 검사 – 이동 거리 0 → 사실상 Overlap
	// TArray<FOverlapResult> OverlapResults;
	// FCollisionQueryParams  Params{ TEXT("WeaponTrace"), false, GetOwner() };
	//
	// bool bHit = GetWorld()->OverlapMultiByChannel(
	// 	OverlapResults,
	// 	CenterLocation,
	// 	SkeletalMeshComponent->GetSocketQuaternion(RotationSocketName),
	// 	ECC_GameTraceChannel1,
	// 	FCollisionShape::MakeBox(BoxHalfExtent),
	// 	Params);
	// if (bHit)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("%d"), OverlapResults.Num());
	// }
}

void UTraceComponent::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}