/**
 * Bag 类，
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDataRow.h"  // 引入数据表行
#include "Bag.generated.h"

/**
 * 背包类继承自 Actor
 */
UCLASS()
class LEARN_2D_01_API ABag : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	ABag();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// 添加物品
	UFUNCTION(BlueprintCallable, Category = "Bag")
	bool AddItem(const FString& ItemType, int32 Quantity);

	// 移除物品
	UFUNCTION(BlueprintCallable, Category = "Bag")
	bool RemoveItem(const FString& ItemType, int32 Quantity);

	// 更新 UI 显示
	UFUNCTION(BlueprintCallable, Category = "Bag")
	void UpdateUI();

	// 初始化背包
	UFUNCTION(BlueprintCallable, Category = "Bag")
	void InitializeBag(int32 DefaultItemCount, const FString& DefaultItemType);

	// 查找物品
	UFUNCTION(BlueprintCallable, Category = "Bag")
	int32 FindItemIndex(const FString& ItemType) const;

private:
	// 背包物品列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
	TArray<FItemDataRow> Items; // 使用 FItemDataRow 替代简单的物品信息

	// 最大容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
	int32 MaxCapacity;

	// 背包UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* BagUI;

	// 显示物品名称UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* DisplayItemUI;
};
