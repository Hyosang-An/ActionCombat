// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = GetOwner<ACharacter>();
	OwnerController = GetWorld()->GetFirstPlayerController();
	MovementComponent = OwnerCharacter->GetCharacterMovement();
	SpringArmComponent = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
}

/**
 * 락온 시스템을 시작합니다.
 *
 * 지정된 반경 내에서 가장 가까운 타겟을 찾아 락온을 시작합니다.
 * 타겟이 발견되면 카메라 회전 입력을 무시하고 캐릭터가 타겟을 자동으로 바라보도록 설정됩니다.
 * 
 * @param Radius 타겟을 검색할 구체 형태의 반경 (기본값: 750)
 */
void ULockonComponent::StartLockon(float Radius)
{
	// 충돌 검사 결과를 저장할 구조체
	FHitResult HitResult;
	// 현재 캐릭터의 위치 
	FVector CurrentLocaton = OwnerCharacter->GetActorLocation();
	// 충돌 검사시 무시할 대상 설정 (자기 자신은 무시)
	FCollisionQueryParams IgnoreParams{ FName{ TEXT("Ignore Collision Params") }, false, OwnerCharacter };

	// 현재 위치에서 구체 모양으로 충돌 검사 수행
	// TODO: 나중에 UWorld::OverlapMultiByChannel이나 UKismetSystemLibrary::SphereOverlapActors 이걸로 원뿔 판정으로 변경하기
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, CurrentLocaton, CurrentLocaton, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius), IgnoreParams);
	if (!HasHit)
	{
		return;
	}

	if (!HitResult.GetActor()->Implements<UEnemy>())
	{
		return;
	}

	// 검출된 액터를 현재 타겟으로 설정
	CurrentTargetActor = HitResult.GetActor();
	// 카메라 회전 입력 무시 설정
	OwnerController->SetIgnoreLookInput(true);
	// 이동 방향으로의 자동 회전 비활성화 
	MovementComponent->bOrientRotationToMovement = false;
	// 컨트롤러 회전값을 따라가도록 설정
	MovementComponent->bUseControllerDesiredRotation = true;
	// 락온된 적에게 선택되었다는 것을 알려주는 인터페이스 메서드를 호출합니다.
	IEnemy::Execute_OnSelect(CurrentTargetActor);

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockonComponent::StopLockon()
{
	if (!IsValid(CurrentTargetActor))
		return;
	
	IEnemy::Execute_OnDeselect(CurrentTargetActor);

	// 1) 락온 해제 직전 카메라의 월드 회전값 저장
	//    PlayerCameraManager를 통해 현재 카메라 회전(월드)을 꺼내올 수 있습니다.
	const FRotator SavedCameraWorldRot = OwnerController->PlayerCameraManager->GetCameraRotation();

	// 2) 락온 타겟 해제
	CurrentTargetActor = nullptr;

	// 3) 입력 리셋
	// 여러 시스템(잠금 조준, 컷씬, UI 등)이 SetIgnoreLookInput(true) 를 쓸 수 있는 상황에서,
	// StopLockon() 안에서 SetIgnoreLookInput(false) 만 쓰면 다른 시스템의 무시 요청이 남아있어 회전 입력이 풀리지 않을 위험이 있습니다.
	// ResetIgnoreLookInput() 를 쓰면 이런 참조 카운트 문제를 모두 없애고, 확실히 마우스·게임패드 회전 입력을 복구할 수 있습니다.
	OwnerController->ResetIgnoreLookInput();

	// 4) 캐릭터 회전 플래그 원복
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->bUseControllerDesiredRotation = false;

	// 5) 스프링암 오프셋만 기본(0)으로 리셋
	SpringArmComponent->TargetOffset = FVector::ZeroVector;

	// 6) 저장해 둔 '락온 해제 직전 카메라 월드 회전'을 컨트롤러에 적용
	OwnerController->SetControlRotation(SavedCameraWorldRot);

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockonComponent::ToggleLockon(float Radius)
{
	if (IsValid(CurrentTargetActor))
	{
		StopLockon();
	}
	else
	{
		StartLockon(Radius);
	}
}

/**
 * LockonComponent의 프레임당 로직을 처리합니다.
 *
 * 이 함수는 매 프레임마다 락온 시스템에 필요한 업데이트를 수행합니다.
 * 소유자 캐릭터와 현재 타겟 액터 사이의 거리를 계산합니다.
 * 타겟이 지정된 락온 해제 거리를 초과하면 락온이 해제되고,
 * 그렇지 않으면 타겟에 초점을 맞추도록 컨트롤 회전값이 업데이트됩니다.  
 * 
 * @param DeltaTime 마지막 틱 이후 경과된 시간.
 * @param TickType 이 컴포넌트가 실행 중인 틱의 유형.
 * @param ThisTickFunction 이 틱 함수에 대한 정보.
 */
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 락온 타겟이 없으면 업데이트 중단
	if (!IsValid(CurrentTargetActor))
	{
		StopLockon();
		return;
	}

	// 캐릭터와 타겟의 현재 위치 얻기 
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = CurrentTargetActor->GetActorLocation();

	// 타겟과의 거리 체크하여 일정 거리 이상이면 락온 해제
	double TargetDistance = FVector::Dist(CurrentLocation, TargetLocation);
	if (TargetDistance > BreakLockonDistance)
	{
		StopLockon();
		return;
	}

	// 타겟 위치를 약간 아래로 조정 (캐릭터 중심점 보정)
	TargetLocation.Z -= 125;

	// 타겟을 향한 새로운 회전값 계산
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	// 위쪽 시야 제한 (-45도)
	NewRotation.Pitch = FMath::Max(NewRotation.Pitch, -45);
	// 계산된 회전값을 컨트롤러에 적용
	OwnerController->SetControlRotation(NewRotation);

	// 락온 시 카메라 높이 조정
	SpringArmComponent->TargetOffset = FVector{ 0, 0, 140 };
}