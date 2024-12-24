/**
 * Bag �࣬
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDataRow.h"  // �������ݱ���
#include "Bag.generated.h"

/**
 * ������̳��� Actor
 */
UCLASS()
class LEARN_2D_01_API ABag : public AActor
{
	GENERATED_BODY()

public:
	// ���캯��
	ABag();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// �����Ʒ
	UFUNCTION(BlueprintCallable, Category = "Bag")
	bool AddItem(const FString& ItemType, int32 Quantity);

	// �Ƴ���Ʒ
	UFUNCTION(BlueprintCallable, Category = "Bag")
	bool RemoveItem(const FString& ItemType, int32 Quantity);

	// ���� UI ��ʾ
	UFUNCTION(BlueprintCallable, Category = "Bag")
	void UpdateUI();

	// ��ʼ������
	UFUNCTION(BlueprintCallable, Category = "Bag")
	void InitializeBag(int32 DefaultItemCount, const FString& DefaultItemType);

	// ������Ʒ
	UFUNCTION(BlueprintCallable, Category = "Bag")
	int32 FindItemIndex(const FString& ItemType) const;

private:
	// ������Ʒ�б�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
	TArray<FItemDataRow> Items; // ʹ�� FItemDataRow ����򵥵���Ʒ��Ϣ

	// �������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bag")
	int32 MaxCapacity;

	// ����UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* BagUI;

	// ��ʾ��Ʒ����UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* DisplayItemUI;
};
