#include "Items.h"
#include "Components/BoxComponent.h"
#include "PaperSprite.h"
#include "Engine/Engine.h"
//���캯�� 
AItems::AItems(UDataTable* InDataTable, FName InRowName)
{
    // ���� Tick
    PrimaryActorTick.bCanEverTick = false;
    
    // ��ʼ�����
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(DefaultSceneRoot);

    PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
    PaperSprite->SetupAttachment(DefaultSceneRoot);

    // �������ݱ��������
    DataTable = InDataTable;
    RowName = InRowName;
     // ��ʼ�� CurrentRow
    if (DataTable && !RowName.IsNone())
    {
        FItemDataRow* Row = DataTable->FindRow<FItemDataRow>(RowName, TEXT(""));
        if (Row)
        {
            CurrentRow = *Row;
        }
    }
}


// ��Ϸ��ʼʱ����
void AItems::BeginPlay()
{
    Super::BeginPlay();

    // ������ݱ���������Ƿ���Ч
    if (DataTable && !RowName.IsNone())
    {
        // ������Ʒ���������
        UpdateSprite();
    }
}

void AItems::SwitchType(const FString& Selection)
{
    if (DataTable && !Selection.IsEmpty())
    {
        // ���ݴ���� Selection �������ݱ��еĶ�Ӧ��
        FItemData* ItemData = DataTable->FindRow<FItemData>(FName(*Selection), TEXT(""));

        if (ItemData)
        {
            // ���µ�ǰ�е� RowName���Ա� UpdateSprite ���Ի���������¾���
            RowName = FName(*Selection);

            // ������Ʒ�ľ���
            UpdateSprite();
        }
    }
}

void AItems::UpdateSprite()
{
    // ���������Ʋ������ݱ��е���
    FItemData* ItemData = DataTable->FindRow<FItemData>(RowName, TEXT(""));

    if (ItemData)
    {
        // ������Ʒ�ľ���
        PaperSprite->SetSprite(ItemData->Sprite);
    }
}


 
