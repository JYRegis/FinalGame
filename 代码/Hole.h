#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hole.generated.h"

UCLASS()
class LEARN_2D_01_API AHole : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AHole();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 是否已经种植
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	bool bIsPlanted;

	// 生长时长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	int32 GrowthDuration;

	// 是否可收获
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	bool bCanBeHarvested;

	// 行名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	FName RowName;

	// S_种子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	FString SeedType;

	// S_类别
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	FString CategoryType;
	
    // Actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AActor> SpawnedActorClass;
    
    // 种子生长阶段
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    int32 SeedGrowthStage;

    // 种子当前Sprite
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    UPaperSprite* CurrentSeedSprite;

    // 显示种子状态
    UFUNCTION()
    void UpdateSeedSprite();
    
    // 作物生长更新
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void UpdateCropGrowth();

    // 定时器句柄
    FTimerHandle GrowthTimerHandle;

    // 作物最大生长阶段
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    int32 MaxGrowthStage;

    // 生长持续时间（每阶段）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    float GrowthDurationPerStage;

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// 设置 Hole 可见性的函数
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void SetVisibility(bool bVisible);

	// 检测物品交互
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void InteractWithItem(const FString& ItemType);

	// 设置 Hole 的状态
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void SetHoleState(const FString& State);

	// 播放定时器操作
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void StartTimer(float Duration);
	
    // 自定义事件：收获
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void Harvest();
    
    // 自定义事件：种植
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void PlantSeed(const FString& SeedType, int32 GrowthStage);
	
	// 自定义事件：浇水
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void WaterCrop();
};


