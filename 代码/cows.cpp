#include "Cows.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// 构造函数
ACows::ACows()
{
	// 启用 Tick
	PrimaryActorTick.bCanEverTick = true;

	// 默认目标位置初始化
	TargetPosition = FVector::ZeroVector;
}

// 游戏开始时调用
void ACows::BeginPlay()
{
	Super::BeginPlay();

	// 初始化目标位置
	SetTargetPosition(GetActorLocation());
}

// 每帧调用
void ACows::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 强制固定旋转值
	SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw, 0.0f));
}

// 启动移动功能
void ACows::StartMovement()
{
	// 设置定时器，用于间隔调用 MoveToTarget 函数
	GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ACows::MoveToTarget, 0.1f, true);
}

// 设置目标位置
void ACows::SetTargetPosition(FVector NewTargetPosition)
{
	TargetPosition = NewTargetPosition;
	UE_LOG(LogTemp, Log, TEXT("Cows SetTargetPosition: New target position set to (%f, %f, %f)."),
		TargetPosition.X, TargetPosition.Y, TargetPosition.Z);
}

// 自定义事件：开始移动
void ACows::BeginCustomMovement()
{
	// 获取玩家的Pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	// 随机生成目标点
	FVector RandomTarget = GenerateRandomTargetLocation();

	// 设置新的目标位置
	SetTargetPosition(RandomTarget);
	if (FVector::Dist(GetActorLocation(), RandomTarget) > 5.0f)
	{
		// 0.2秒后移动
		GetWorldTimerManager().SetTimer(DelayTimerHandle, [this]()
		{
			this->StartMovement();
		}, 0.2f, false);
	}
}

// AI 移动功能
void ACows::MoveToTarget()
{
	if (FVector::Dist(GetActorLocation(), TargetPosition) > 5.0f)
	{
		// 计算移动方向
		FVector Direction = (TargetPosition - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + Direction * 100.0f * GetWorld()->GetDeltaSeconds();

		// 更新位置
		SetActorLocation(NewLocation);
	}
	else
	{
		// 停止移动
		GetWorldTimerManager().ClearTimer(MovementTimerHandle);
	}
}

// 随机生成目标点
FVector ACows::GenerateRandomTargetLocation()
{
	// 生成随机目标点
	float RandomX = FMath::RandRange(-1000.0f, 1000.0f);
	float RandomY = FMath::RandRange(-1000.0f, 1000.0f);
	float RandomZ = GetActorLocation().Z; // 保持Z值不变

	return FVector(RandomX, RandomY, RandomZ);
}

