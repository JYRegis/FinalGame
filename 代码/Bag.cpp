#include "Bag.h"
#include "BagUI.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"

// ���캯��
ABag::ABag()
{
	PrimaryActorTick.bCanEverTick = true;

	// ����Ĭ���������
	MaxCapacity = 20; // ����Ĭ���������Ϊ 20
	BagUI = nullptr;  // ��ʼ�� BagUI
	DisplayItemUI = nullptr; // ��ʼ����Ʒ��ʾ UI
}

void ABag::BeginPlay()
{
	Super::BeginPlay();

	// ��ȡӵ���ߣ�Owner��
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwningPawn->GetController());
		if (PlayerController)
		{
			// ��̬���� UI չʾ��Ʒ���ƿؼ�
			if (DisplayItemUIClass)
			{
				DisplayItemUI = CreateWidget<UUserWidget>(PlayerController, DisplayItemUIClass);
				if (DisplayItemUI)
				{
					// ����Ʒ���� UI ��ӵ���ͼ
					DisplayItemUI->AddToViewport();
				}
			}

			// ��̬�������� UI �ؼ�
			if (BagUIClass)
			{
				BagUI = CreateWidget<UBagUI>(PlayerController, BagUIClass);
				if (BagUI)
				{
					// ������ UI ��ӵ���ͼ
					BagUI->AddToViewport();
					// ��ʼ�������� UI �İ�
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

// ��ʼ������
void ABag::InitializeBag(int32 DefaultItemCount, const FString& DefaultItemType)
{
	for (int32 i = 0; i < DefaultItemCount && Items.Num() < MaxCapacity; i++)
	{
		FBagItem NewItem;
		NewItem.ItemType = DefaultItemType;
		NewItem.Quantity = 1; // Ĭ������Ϊ 1
		Items.Add(NewItem);
	}

	UpdateUI(); // ��ʼ����ɺ���� UI ��ʾ
}

// ���� UI ��ʾ
void ABag::UpdateUI()
{
	if (BagUI)
	{
		BagUI->ClearBagContents(); // ������� UI ��ʾ

		// ����������Ʒ������ UI
		for (const FBagItem& Item : Items)
		{
			BagUI->AddItemToUI(Item.ItemType, Item.Quantity);
		}
	}
}

// �����Ʒ
bool ABag::AddItem(const FString& ItemType, int32 Quantity)
{
	if (IsFull())
	{
		// �����������޷��������Ʒ
		return false;
	}

	int32 ItemIndex = FindItemIndex(ItemType);
	if (ItemIndex != INDEX_NONE)
	{
		// �����Ʒ�Ѿ����ڣ�����������
		Items[ItemIndex].Quantity += Quantity;
	}
	else
	{
		// ����δ������Ʒ�����ڣ��������Ʒ
		FBagItem NewItem;
		NewItem.ItemType = ItemType;
		NewItem.Quantity = Quantity;
		Items.Add(NewItem);
	}

	UpdateUI(); // ���� UI ��ʾ
	return true;
}

// �Ƴ���Ʒ
bool ABag::RemoveItem(const FString& ItemType, int32 Quantity)
{
	int32 ItemIndex = FindItemIndex(ItemType);
	if (ItemIndex == INDEX_NONE)
	{
		// δ�ҵ���Ʒ
		return false;
	}

	if (Items[ItemIndex].Quantity < Quantity)
	{
		// ��������
		return false;
	}

	// �۳���Ʒ������
	Items[ItemIndex].Quantity -= Quantity;

	if (Items[ItemIndex].Quantity <= 0)
	{
		// �����Ʒ����Ϊ 0���Ƴ���Ʒ
		Items.RemoveAt(ItemIndex);
	}

	UpdateUI(); // ���� UI ��ʾ
	return true;
}
// �Ƴ���Ʒ������ʵ��
bool ABag::RemoveItemAndSpawnEntity(const FString& ItemType, int32 Quantity, FVector SpawnLocation, FRotator SpawnRotation)
{
	// ������Ʒ����
	int32 ItemIndex = FindItemIndex(ItemType);
	if (ItemIndex == INDEX_NONE)
	{
		// δ�ҵ���Ʒ
		return false;
	}

	// ����Ƿ����㹻������
	if (Items[ItemIndex].Quantity < Quantity)
	{
		// �������㣬����ʧ��
		return false;
	}

	// �ۼ�����
	Items[ItemIndex].Quantity -= Quantity;

	// �����Ʒ����Ϊ 0����ӱ������Ƴ�
	if (Items[ItemIndex].Quantity <= 0)
	{
		Items.RemoveAt(ItemIndex);
	}

	UpdateUI(); // ���±��� UI

	// ������Ʒ����������Ʒʵ��
	if (ItemSpawnMap.Contains(ItemType))
	{
		TSubclassOf<AActor> ItemClass = ItemSpawnMap[ItemType];
		if (ItemClass)
		{
			// ��ָ��λ��������Ʒʵ��
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);

			if (SpawnedItem)
			{
				// ����������Ʒ���������������ɵ���Ʒ�� `SetQuantity` ������
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
// ������Ʒ
int32 ABag::FindItemIndex(const FString& ItemType) const
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemType == ItemType)
		{
			return i;
		}
	}
	return INDEX_NONE; // δ�ҵ�
}

// �ó���Ʒ����
void ABag::TakeOutItem(FString CurrentTool, UObject* TargetObject)
{
	// ��鵱ǰ�����Ƿ��Ѿ���Ŀ�깤��
	if (ActiveTool == CurrentTool)
	{
		// ��ǰ�����Ѿ���Ŀ�깤�ߣ�����ı�
		return;
	}

	// ��֤Ŀ������Ƿ���Ч
	if (IsValid(TargetObject))
	{
		// ���õ�ǰ����
		ActiveTool = CurrentTool;

		// ���߿��Խ���
		bIsToolActive = true;
	}
	else
	{
		// �����޷�����
		bIsToolActive = false;
	}
}


// ����Ƿ�ﵽ��������
bool ABag::IsFull() const
{
	return Items.Num() >= MaxCapacity;
}
