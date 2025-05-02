// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StatsComponent.h"

#include "Interfaces/Fighter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ReduceHealth(float Amount, AActor* Opponent)
{
	IFighter* FighterInterface = Cast<IFighter>(GetOwner());
	if (FighterInterface && !FighterInterface->CanTakeDamage(Opponent))
	{
		return;
	}
	
	if (Stats.Contains(EStat::Health) && Stats.Contains(EStat::MaxHealth))
	{
		if (Stats[EStat::Health] <= 0)
			return;

		Stats[EStat::Health] -= Amount;
		Stats[EStat::Health] = FMath::Clamp(Stats[EStat::Health], 0.0f, Stats[EStat::MaxHealth]);

		OnHealthPercentageUpdateDelegate.Broadcast(GetStatPercentage(EStat::Health, EStat::MaxHealth));

		if (Stats[EStat::Health] == 0)
			OnZeroHealthDelegate.Broadcast();
	}
}

void UStatsComponent::ReduceStamina(float Amount)
{
	if (Stats.Contains(EStat::Stamina) && Stats.Contains(EStat::MaxStamina))
	{
		if (Stats[EStat::Stamina] <= 0)
			return;

		Stats[EStat::Stamina] -= Amount;
		Stats[EStat::Stamina] = FMath::Clamp(Stats[EStat::Stamina], 0.0f, Stats[EStat::MaxStamina]);

		OnStaminaPercentageUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
	}

	bCanRegenStamina = false;

	// 타이머 방식 대신 사용 가능
	// FLatentActionInfo LatentActionInfo{ 0, 100, TEXT("EnableRegenStamina"), this };
	// UKismetSystemLibrary::RetriggerableDelay(this, StaminaDelayDuration, LatentActionInfo);

	// 타이머 방식
	// 새로운 타이머 등록 (RegenStaminaTimerHandle에 이미 set된 타이머가 있다면 Clear하고 다시 setting)
	GetWorld()->GetTimerManager().SetTimer(RegenStaminaTimerHandle, this, &UStatsComponent::EnableRegenStamina, StaminaDelayDuration, false);
}

void UStatsComponent::RegenStamina()
{
	if (!bCanRegenStamina)
		return;

	if (!Stats.Contains(EStat::Stamina))
		return;

	Stats[EStat::Stamina] = UKismetMathLibrary::FInterpTo_Constant(Stats[EStat::Stamina], Stats[EStat::MaxStamina], GetWorld()->GetDeltaSeconds(), StaminaRegenRate);
	OnStaminaPercentageUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}

void UStatsComponent::EnableRegenStamina()
{
	bCanRegenStamina = true;
}

float UStatsComponent::GetStatPercentage(EStat Current, EStat Max)
{
	if (!Stats.Contains(Current) || !Stats.Contains(Max))
		return 0.0f;

	return Stats[Current] / Stats[Max];
}