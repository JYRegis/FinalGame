#include "RandomRock.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AssertionMacros.h"

// ���캯��
ARandomRock::ARandomRock(){    // ���� Tick
    PrimaryActorTick.bCanEverTick = false;
}

// ��Ϸ��ʼʱ����
void ARandomRock::BeginPlay(){
    Super::BeginPlay();
    try{
        // ����Ƿ�������ʯͷ����
        if (!RockClass){
            throw std::runtime_error("δ����ʯͷ");
        }

        // ѭ������ָ��������ʯͷ
        for (int32 i = 0; i < RockCount; i++){
            // �������λ��
            FVector SpawnLocation = GenerateRandomPosition();

            // ���� Actor �ı任
            FRotator SpawnRotation = FRotator::ZeroRotator;
            FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

            // ��������ʯͷ
            AActor* SpawnedRock = GetWorld()->SpawnActor<AActor>(RockClass, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
            if (!SpawnedRock){
                throw std::runtime_error("����");
            }
        }
    }
    catch (const std::exception& e){        // ���񲢼�¼����
        UE_LOG(LogTemp, Error, TEXT("����: %s"), *FString(e.what()));
    }
}

// �������λ��
FVector ARandomRock::GenerateRandomPosition(){
    try{        // ������� X��Y��Z λ��
        float RandomX = FMath::FRandRange(MinX, MaxX);
        float RandomY = FMath::FRandRange(MinY, MaxY);
        float RandomZ = FMath::FRandRange(MinZ, MaxZ);

        return FVector(RandomX, RandomY, RandomZ);
    }
    catch (...){     // �����κ�δ֪���󣬷���Ĭ��λ��
        UE_LOG(LogTemp, Warning, TEXT("���󣬷��س�ʼλ�ã�"));
        return FVector(0.0f, 0.0f, 0.0f);
    }
}
