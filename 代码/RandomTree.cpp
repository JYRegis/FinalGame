#include "RandomTree.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// ���캯��
ARandomTree::ARandomTree()
{
	// ���� Tick
	PrimaryActorTick.bCanEverTick = false;
}

// ��Ϸ��ʼʱ����
void ARandomTree::BeginPlay()
{
	Super::BeginPlay();

	// ����Ƿ�������������
	if (!TreeClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("δ������"));
		return;
	}
	try {
		// ѭ������ָ����������
		for (int32 i = 0; i < TreeCount; i++)
		{
			// �������λ��
			FVector SpawnLocation = GenerateRandomPosition();

			// ���� Actor �ı任
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

			// ������
			AActor* SpawnedTree = GetWorld()->SpawnActor<AActor>(TreeClass, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
			if (!SpawnedTree) {
				throw std::runtime_error("����");
			}
		}

	}
	catch (const std::exception& e) {        // ���񲢼�¼����
		UE_LOG(LogTemp, Error, TEXT("����: %s"), *FString(e.what()));
	}

}

// �������λ��
FVector ARandomTree::GenerateRandomPosition()
{
	// ������� X��Y��Z λ��
	float RandomX = FMath::FRandRange(MinX, MaxX);
	float RandomY = FMath::FRandRange(MinY, MaxY);
	float RandomZ = FMath::FRandRange(MinZ, MaxZ);

	return FVector(RandomX, RandomY, RandomZ);
}
