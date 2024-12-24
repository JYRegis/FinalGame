#include "RandomTree.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// 构造函数
ARandomTree::ARandomTree()
{
	// 禁用 Tick
	PrimaryActorTick.bCanEverTick = false;
}

// 游戏开始时调用
void ARandomTree::BeginPlay()
{
	Super::BeginPlay();

	// 检查是否已设置树的类
	if (!TreeClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("未设置树"));
		return;
	}
	try {
		// 循环生成指定数量的树
		for (int32 i = 0; i < TreeCount; i++)
		{
			// 生成随机位置
			FVector SpawnLocation = GenerateRandomPosition();

			// 生成 Actor 的变换
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

			// 生成树
			AActor* SpawnedTree = GetWorld()->SpawnActor<AActor>(TreeClass, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
			if (!SpawnedTree) {
				throw std::runtime_error("错误");
			}
		}

	}
	catch (const std::exception& e) {        // 捕获并记录错误
		UE_LOG(LogTemp, Error, TEXT("错误: %s"), *FString(e.what()));
	}

}

// 生成随机位置
FVector ARandomTree::GenerateRandomPosition()
{
	// 随机生成 X、Y、Z 位置
	float RandomX = FMath::FRandRange(MinX, MaxX);
	float RandomY = FMath::FRandRange(MinY, MaxY);
	float RandomZ = FMath::FRandRange(MinZ, MaxZ);

	return FVector(RandomX, RandomY, RandomZ);
}
