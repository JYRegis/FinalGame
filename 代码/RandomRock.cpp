#include "RandomRock.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AssertionMacros.h"

// 构造函数
ARandomRock::ARandomRock(){    // 禁用 Tick
    PrimaryActorTick.bCanEverTick = false;
}

// 游戏开始时调用
void ARandomRock::BeginPlay(){
    Super::BeginPlay();
    try{
        // 检查是否已设置石头的类
        if (!RockClass){
            throw std::runtime_error("未设置石头");
        }

        // 循环生成指定数量的石头
        for (int32 i = 0; i < RockCount; i++){
            // 生成随机位置
            FVector SpawnLocation = GenerateRandomPosition();

            // 生成 Actor 的变换
            FRotator SpawnRotation = FRotator::ZeroRotator;
            FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

            // 尝试生成石头
            AActor* SpawnedRock = GetWorld()->SpawnActor<AActor>(RockClass, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
            if (!SpawnedRock){
                throw std::runtime_error("错误");
            }
        }
    }
    catch (const std::exception& e){        // 捕获并记录错误
        UE_LOG(LogTemp, Error, TEXT("错误: %s"), *FString(e.what()));
    }
}

// 生成随机位置
FVector ARandomRock::GenerateRandomPosition(){
    try{        // 随机生成 X、Y、Z 位置
        float RandomX = FMath::FRandRange(MinX, MaxX);
        float RandomY = FMath::FRandRange(MinY, MaxY);
        float RandomZ = FMath::FRandRange(MinZ, MaxZ);

        return FVector(RandomX, RandomY, RandomZ);
    }
    catch (...){     // 处理任何未知错误，返回默认位置
        UE_LOG(LogTemp, Warning, TEXT("错误，返回初始位置！"));
        return FVector(0.0f, 0.0f, 0.0f);
    }
}
