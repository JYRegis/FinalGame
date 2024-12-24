#include "Items.h"
#include "Components/BoxComponent.h"
#include "PaperSprite.h"
#include "Engine/Engine.h"
//构造函数 
AItems::AItems(UDataTable* InDataTable, FName InRowName)
{
    // 启用 Tick
    PrimaryActorTick.bCanEverTick = false;
    
    // 初始化组件
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(DefaultSceneRoot);

    PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
    PaperSprite->SetupAttachment(DefaultSceneRoot);

    // 设置数据表和行名称
    DataTable = InDataTable;
    RowName = InRowName;
     // 初始化 CurrentRow
    if (DataTable && !RowName.IsNone())
    {
        FItemDataRow* Row = DataTable->FindRow<FItemDataRow>(RowName, TEXT(""));
        if (Row)
        {
            CurrentRow = *Row;
        }
    }
}


// 游戏开始时调用
void AItems::BeginPlay()
{
    Super::BeginPlay();

    // 检查数据表和行名称是否有效
    if (DataTable && !RowName.IsNone())
    {
        // 更新物品精灵和属性
        UpdateSprite();
    }
}

void AItems::SwitchType(const FString& Selection)
{
    if (DataTable && !Selection.IsEmpty())
    {
        // 根据传入的 Selection 查找数据表中的对应行
        FItemData* ItemData = DataTable->FindRow<FItemData>(FName(*Selection), TEXT(""));

        if (ItemData)
        {
            // 更新当前行的 RowName，以便 UpdateSprite 可以基于这个更新精灵
            RowName = FName(*Selection);

            // 更新物品的精灵
            UpdateSprite();
        }
    }
}

void AItems::UpdateSprite()
{
    // 根据行名称查找数据表中的行
    FItemData* ItemData = DataTable->FindRow<FItemData>(RowName, TEXT(""));

    if (ItemData)
    {
        // 更新物品的精灵
        PaperSprite->SetSprite(ItemData->Sprite);
    }
}


 
