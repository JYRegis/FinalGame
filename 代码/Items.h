#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "Items.generated.h"

// 数据表行结构
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
 * Items 类
 */
UCLASS()
class LEARN_2D_01_API AItems : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AItems(UDataTable* InDataTable, FName InRowName);

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick() override;
	// 数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* DataTable;

	// 数据表行名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName RowName;

	// Box 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Box;

	// PaperSprite 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* PaperSprite;

	// 切换类型函数
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SwitchType(const FString& Selection);
	
    // 当前行数据
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
    FItemDataRow CurrentRow;
    
private:
    // 切换物品类型函数
    void SwitchType(const FString& Selection);

    // 更新精灵
    void UpdateSprite();
    
};
