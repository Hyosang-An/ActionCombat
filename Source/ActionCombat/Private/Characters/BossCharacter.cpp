// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Characters/BossCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/MainCharacter.h"
#include "Characters/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	BlackboardComp = AIController->GetBlackboardComponent();

	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), InitialState);

	GetWorld()->GetFirstPlayerController()->GetPawn<AMainCharacter>()->StatsComp->OnZeroHealthDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("CurrentState")));

	if (DetectedPawn != PawnToDetect || CurrentState != EEnemyState::Idle)
		return;

	BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);
}

float ABossCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

void ABossCharacter::Attack()
{
	CombatComp->RandomAttack();

}

float ABossCharacter::GetAnimationDuration()
{
	return CombatComp->AnimationDuration;
}

float ABossCharacter::GetMeleeRange()
{
	return StatsComp->Stats[EStat::MeleeRange];
}

void ABossCharacter::HandlePlayerDeath()
{
	AIController->GetBlackboardComponent()->SetValueAsEnum(TEXT("Currentstate"), EEnemyState::GameOver);
}

void ABossCharacter::HandleDeath()
{
	float Duration = PlayAnimMontage(DeathAnimMontage);
	AIController->GetBrainComponent()->StopLogic("defeated");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABossCharacter::FinishDeathAnim, Duration + 4, false);

	if (IMainPlayer* MainPlayerInterface = Cast<IMainPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter()))
	{
		MainPlayerInterface->EndLockonWithActor(this);
	}
}

void ABossCharacter::FinishDeathAnim()
{
	Destroy();
}