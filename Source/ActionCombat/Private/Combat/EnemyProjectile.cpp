// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectile.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
	APawn* Pawn = Cast<APawn>(OtherActor);

	if (!IsValid(Pawn) || !Pawn->IsPlayerControlled())
		return;
	
	FDamageEvent ProjectileAttackEvent;
	Pawn->TakeDamage(Damage, ProjectileAttackEvent, GetInstigatorController(), this);

	// FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitTemplate);
	// FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();
	//
	// FTimerHandle DeathTimerHandle;
	// GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyProjectile::DestroyProjectile, 0.5f);

	// SetActorEnableCollision(false);

	// 더 간단한 방법...
	UGameplayStatics::SpawnEmitterAtLocation(this, HitTemplate, GetActorLocation(), GetActorRotation());
	Destroy();
}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

