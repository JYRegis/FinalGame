#include "Cows.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// ���캯��
ACows::ACows()
{
	// ���� Tick
	PrimaryActorTick.bCanEverTick = true;

	// Ĭ��Ŀ��λ�ó�ʼ��
	TargetPosition = FVector::ZeroVector;
}

// ��Ϸ��ʼʱ����
void ACows::BeginPlay()
{
	Super::BeginPlay();

	// ��ʼ��Ŀ��λ��
	SetTargetPosition(GetActorLocation());
}

// ÿ֡����
void ACows::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ǿ�ƹ̶���תֵ
	SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
}

// �����ƶ�����
void ACows::StartMovement()
{
	// ���ö�ʱ�������ڼ������ MoveToTarget ����
	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ACows::MoveToTarget, 0.1f, true);
}

// ����Ŀ��λ��
void ACows::SetTargetPosition(FVector NewTargetPosition)
{
	TargetPosition = NewTargetPosition;
	UE_LOG(LogTemp, Log, TEXT("Cows SetTargetPosition: New target position set to (%f, %f, %f)."),
		TargetPosition.X, TargetPosition.Y, TargetPosition.Z);
}

// �Զ����¼�����ʼ�ƶ�
void ACows::BeginCustomMovement()
{
	// ��ȡ��ҵ�Pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	// �������Ŀ���
	FVector RandomTarget = GenerateRandomTargetLocation();

	// �����µ�Ŀ��λ��
	SetTargetPosition(RandomTarget);
	if (FVector::Dist(GetActorLocation(), RandomTarget) > 5.0f)
	{
		// 0.2����ƶ�
		GetWorldTimerManager().SetTimer(DelayTimerHandle, [this]()
		{
			this->StartMovement();
		}, 0.2f, false);
	}
}

// AI �ƶ�����
void ACows::MoveToTarget()
{
	if (FVector::Dist(GetActorLocation(), TargetPosition) > 5.0f)
	{
		// �����ƶ�����
		FVector Direction = (TargetPosition - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + Direction * 100.0f * GetWorld()->GetDeltaSeconds();

		// ����λ��
		SetActorLocation(NewLocation);
	}
	else
	{
		// ֹͣ�ƶ�
		GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	}
}

// �������Ŀ���
FVector ACows::GenerateRandomTargetLocation()
{
	// �������Ŀ���
	float RandomX = FMath::RandRange(-1000.0f, 1000.0f);
	float RandomY = FMath::RandRange(-1000.0f, 1000.0f);
	float RandomZ = GetActorLocation().Z; // ����Zֵ����

	return FVector(RandomX, RandomY, RandomZ);
}

