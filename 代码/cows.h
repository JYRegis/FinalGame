#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "GameFramework/Actor.h"
#include "Cows.generated.h"

UCLASS()
class LEARN_2D_01_API ACows : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// 构造函数
	ACows();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// 启动移动功能
	UFUNCTION(BlueprintCallable, Category = "Cows")
	void StartMovement();

	// 定时器回调函数
	void TimerCallback();

	// 设置目标位置
	UFUNCTION(BlueprintCallable, Category = "Cows")
	void SetTargetPosition(FVector NewTargetPosition);

private:
	// 动物目标位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cows", meta = (AllowPrivateAccess = "true"))
	FVector TargetPosition;

	// 定时器句柄
	FTimerHandle MovementTimerHandle;

	// AI移动函数
	void MoveToTarget();

	// 动画实例指针
	UPROPERTY()
	class UAnimInstance* AnimInstance;

	// 生成的BP实例指针
	UPROPERTY()
	AActor* SpawnedBPInstance;
};

