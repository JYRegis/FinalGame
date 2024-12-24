#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomRock.generated.h"

UCLASS()
class LEARN_2D_01_API ARandomRock : public AActor
{
	GENERATED_BODY()

public:
	// ���캯��
	ARandomRock();

protected:
	virtual void BeginPlay() override;

public:
	// ʯͷ���ࣨ��������ʵ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock")
	TSubclassOf<AActor> RockClass;

	// ��С�����λ�÷�Χ��X��Y��Z��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float MinX = -700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float MaxX = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float MinY = -200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float MaxY = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float MinZ = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	float MaxZ = 10.0f;

	// ʯͷ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock")
	int32 RockCount = 10;

private:
	// �������λ��
	FVector GenerateRandomPosition();
};

