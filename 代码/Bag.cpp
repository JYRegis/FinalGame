#include "Bag.h"
#include "BagUI.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"

// 构造函数
ABag::ABag()
{
	PrimaryActorTick.bCanEverTick = true;

	// 设置默认最大容量
	MaxCapacity = 20; // 假设默认最大容量为 20
	BagUI = nullptr;  // 初始化 BagUI
	DisplayItemUI = nullptr; // 初始化物品显示 UI
}

void ABag::BeginPlay()
{
	Super::BeginPlay();

	// 获取拥有者（Owner）
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwningPawn->GetController());
		if (PlayerController)
		{
			// 动态创建 UI 展示物品名称控件
			if (DisplayItemUIClass)
			{
				DisplayItemUI = CreateWidget<UUserWidget>(PlayerController, DisplayItemUIClass);
				if (DisplayItemUI)
				{
					// 将物品名称 UI 添加到视图
					DisplayItemUI->AddToViewport();
				}
			}

			// 动态创建背包 UI 控件
			if (BagUIClass)
			{
				BagUI = CreateWidget<UBagUI>(PlayerController, BagUIClass);
				if (BagUI)
				{
					// 将背包 UI 添加到视图
					BagUI->AddToViewport();
					// 初始化背包与 UI 的绑定
					BagUI->InitializeBagUI(this);
				}
			}
		}
	}
}

void ABag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 初始化背包
void ABag::InitializeBag(int32 DefaultItemCount, const FString& DefaultItemType)
{
	for (int32 i = 0; i < DefaultItemCount && Items.Num() < MaxCapacity; i++)
	{
		FBagItem NewItem;
		NewItem.ItemType = DefaultItemType;
		NewItem.Quantity = 1; // 默认数量为 1
		Items.Add(NewItem);
	}

	UpdateUI(); // 初始化完成后更新 UI 显示
}

// 更新 UI 显示
void ABag::UpdateUI()
{
	if (BagUI)
	{
		BagUI->ClearBagContents(); // 清空现有 UI 显示

		// 遍历背包物品并更新 UI
		for (const FBagItem& Item : Items)
		{
			BagUI->AddItemToUI(Item.ItemType, Item.Quantity);
		}
	}
}

// 添加物品
bool ABag::AddItem(const FString& ItemType, int32 Quantity)
{
	if (IsFull())
	{
		// 背包已满，无法再添加物品
		return false;
	}

	int32 ItemIndex = FindItemIndex(ItemType);
	if (ItemIndex != INDEX_NONE)
	{
		// 如果物品已经存在，则增加数量
		Items[ItemIndex].Quantity += Quantity;
	}
	else
	{
		// 背包未满且物品不存在，添加新物品
		FBagItem NewItem;
		NewItem.ItemType = ItemType;
		NewItem.Quantity = Quantity;
		Items.Add(NewItem);
	}

	UpdateUI(); // 更新 UI 显示
	return true;
}

// 移除物品
bool ABag::RemoveItem(const FString& ItemType, int32 Quantity)
{
	int32 ItemIndex = FindItemIndex(ItemType);
	if (ItemIndex == INDEX_NONE)
	{
		// 未找到物品
		return false;
	}

	if (Items[ItemIndex].Quantity < Quantity)
	{
		// 数量不足
		return false;
	}

	// 扣除物品的数量
	Items[ItemIndex].Quantity -= Quantity;

	if (Items[ItemIndex].Quantity <= 0)
	{
		// 如果物品数量为 0，移除物品
		Items.RemoveAt(ItemIndex);
	}

	UpdateUI(); // 更新 UI 显示
	return true;
}
// 移除物品并生成实体
bool ABag::RemoveItemAndSpawnEntity(const FString& ItemType, int32 Quantity, FVector SpawnLocation, FRotator SpawnRotation)
{
	// 查找物品索引
	int32 ItemIndex = FindItemIndex(ItemType);
	if (ItemIndex == INDEX_NONE)
	{
		// 未找到物品
		return false;
	}

	// 检查是否有足够的数量
	if (Items[ItemIndex].Quantity < Quantity)
	{
		// 数量不足，返回失败
		return false;
	}

	// 扣减数量
	Items[ItemIndex].Quantity -= Quantity;

	// 如果物品数量为 0，则从背包中移除
	if (Items[ItemIndex].Quantity <= 0)
	{
		Items.RemoveAt(ItemIndex);
	}

	UpdateUI(); // 更新背包 UI

	// 根据物品类型生成物品实体
	if (ItemSpawnMap.Contains(ItemType))
	{
		TSubclassOf<AActor> ItemClass = ItemSpawnMap[ItemType];
		if (ItemClass)
		{
			// 在指定位置生成物品实体
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);

			if (SpawnedItem)
			{
				// 设置生成物品的数量（假设生成的物品有 `SetQuantity` 函数）
				ISpawnableItemInterface* SpawnableItem = Cast<ISpawnableItemInterface>(SpawnedItem);
				if (SpawnableItem)
				{
					SpawnableItem->SetQuantity(Quantity);
					SpawnableItem->SetItemType(ItemType);
				}

				return true;
			}
		}
	}

	return false;
}
// 查找物品
int32 ABag::FindItemIndex(const FString& ItemType) const
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemType == ItemType)
		{
			return i;
		}
	}
	return INDEX_NONE; // 未找到
}

// 拿出物品功能
void ABag::TakeOutItem(FString CurrentTool, UObject* TargetObject)
{
	// 检查当前工具是否已经是目标工具
	if (ActiveTool == CurrentTool)
	{
		// 当前工具已经是目标工具，无需改变
		return;
	}

	// 验证目标对象是否有效
	if (IsValid(TargetObject))
	{
		// 设置当前工具
		ActiveTool = CurrentTool;

		// 工具可以交互
		bIsToolActive = true;
	}
	else
	{
		// 工具无法交互
		bIsToolActive = false;
	}
}


// 检查是否达到容量上限
bool ABag::IsFull() const
{
	return Items.Num() >= MaxCapacity;
}
