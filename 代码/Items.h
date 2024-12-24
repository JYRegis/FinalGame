#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "Items.generated.h"

// ���ݱ��нṹ
USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    class UPaperSprite* Sprite;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCanStack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 StackCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsPaper;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString SeedType;
};
/**
 * Items ��
 */
UCLASS()
class LEARN_2D_01_API AItems : public AActor
{
	GENERATED_BODY()

public:
	// ���캯��
	AItems(UDataTable* InDataTable, FName InRowName);

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick() override;
	// ���ݱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* DataTable;

	// ���ݱ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName RowName;

	// Box ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Box;

	// PaperSprite ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* PaperSprite;

	// �л����ͺ���
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SwitchType(const FString& Selection);
	
    // ��ǰ������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
    FItemDataRow CurrentRow;
    
private:
    // �л���Ʒ���ͺ���
    void SwitchType(const FString& Selection);

    // ���¾���
    void UpdateSprite();
    
};
