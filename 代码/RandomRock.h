#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomRock.generated.h"

UCLASS()
class LEARN_2D_01_API ARandomRock : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	ARandomRock();

protected:
	virtual void BeginPlay() override;

public:
	// 石头的类（用于生成实例）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock")
	TSubclassOf<AActor> RockClass;

	// 最小和最大位置范围（X、Y、Z）
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

	// 石头生成数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock")
	int32 RockCount = 10;

private:
	// 生成随机位置
	FVector GenerateRandomPosition();
};

