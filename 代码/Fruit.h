#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Fruit.generated.h"

/**
 * Fruit 类，继承自 AItems
 */
UCLASS()
class LEARN_2D_01_API AFruit : public AItems
{
	GENERATED_BODY()

public:
	// 构造函数
	AFruit();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

public:
	
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

    virtual void Tick() override;
    
	// 检测碰撞的 Box 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FruitBox;

	// 显示水果的 PaperSprite 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* FruitSprite;

	// 切换水果类型
	UFUNCTION(BlueprintCallable, Category = "Fruit")
	void SwitchFruitType(const FString& FruitType);

private:
	// 控制水果移动动画
	void MoveFruit(const FVector& Direction, float Speed);

	// 控制水果可见性
	void SetFruitVisibility(bool bVisible);

	// 设置水果位置
	void SetFruitLocation(const FVector& NewLocation);

	// 检查水果有效性
	bool IsValidFruit() const;

	// 控制水果跳动动画（使用时间轴）
	void HandleJumpProgress(float Value);
	void OnJumpFinished();

	// 用于跳动动画的时间轴
	FTimeline JumpTimeline;

	// 跳动动画的高度
	UPROPERTY(EditAnywhere, Category = "Animation")
	float JumpHeight;

	// 跳动动画的持续时间
	UPROPERTY(EditAnywhere, Category = "Animation")
	float JumpDuration;

	// 当前水果类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString CurrentFruitType;

	// 跳动是否完成的标志
	bool bShouldMove;

};



