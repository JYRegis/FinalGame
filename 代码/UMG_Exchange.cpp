#include "UMG_Exchange.h"
#include "UMG_Rs.h"
#include "PS_PLAY.h"
#include "PC_PLAY.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "ResourceStruct.h"

UMG_Exchange::UMG_Exchange(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // ������Դ��Ŀ����ͼ
    static ConstructorHelpers::FClassFinder<UUMG_Rs> ResourceItemBPClass(TEXT("/Game/UI/WBP_RS"));
    if (ResourceItemBPClass.Succeeded())
    {
        ResourceItemClass = ResourceItemBPClass.Class;
    }
}

void UMG_Exchange::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ʼ��ʱ���½���վ
    FUN_Update_Exchange();
}

// -------- �ϷŹ���ʵ�� --------

bool UMG_Exchange::OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
    // ��֤�Ϸŵ���Դ
    UUMG_Rs* DroppedResource = nullptr;
    if (!ValidateDropOperation(Operation, DroppedResource))
    {
        return false;
    }

    // ��ȡ��Դ��ActorID��Index
    int32 ActorID = DroppedResource->GetActorID();
    int32 Index = DroppedResource->GetIndex();

    // ������Դ����
    return HandleResourceSell(DroppedResource, ActorID, Index);
}

bool UMG_Exchange::ValidateDropOperation(UDragDropOperation* Operation, UUMG_Rs*& OutResource) const
{
    if (!Operation || !Operation->Payload)
    {
        return false;
    }

    OutResource = Cast<UUMG_Rs>(Operation->Payload);
    if (!OutResource)
    {
        return false;
    }

    return (GetPlayerState() != nullptr && GetPlayerController() != nullptr);
}

bool UMG_Exchange::HandleResourceSell(UUMG_Rs* DroppedResource, int32 ActorID, int32 Index)
{
    APS_PLAY* PlayerState = GetPlayerState();
    APC_PLAY* PC = GetPlayerController();
    
    if (!PlayerState || !PC)
    {
        return false;
    }

    // ���÷����������¼�
    PlayerState->EVE_Sell_Rs(PC, ActorID, Index, PC->GetVector());
    return true;
}

// -------- ���¹���ʵ�� --------

void UMG_Exchange::FUN_Update_Exchange()
{
    // ��ȡ��ҿ�����
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return;
    }

    // ��ȡActor ID
    int32 ActorID = PC->GetActorID();

    // ����������Ŀ
    ClearExistingItems();

    // ��ȡ��Դ�б�
    TArray<FResourceStruct> Resources;
    if (APS_PLAY* PlayerState = GetPlayerState())
    {
        Resources = PlayerState->GetExchangeResources(ActorID);
    }

    // �����µ���Դ��Ŀ
    CreateResourceItems(Resources, ActorID);
}

void UMG_Exchange::ClearExistingItems()
{
    if (WrapBox_42)
    {
        WrapBox_42->ClearChildren();
    }
    ExchangeItems.Empty();
}

void UMG_Exchange::CreateResourceItems(const TArray<FResourceStruct>& Resources, int32 ActorID)
{
    if (!WrapBox_42 || !ResourceItemClass)
    {
        return;
    }

    // Ϊÿ����Դ����UI��Ŀ
    for (int32 Index = 0; Index < Resources.Num(); ++Index)
    {
        // ������Դ�ؼ�
        UUMG_Rs* ResourceWidget = CreateWidget<UUMG_Rs>(this, ResourceItemClass);
        if (ResourceWidget)
        {
            // ������Դ��Ϣ
            SetResourceInfo(ResourceWidget, Resources[Index], ActorID, Index);

            // ��ӵ�WrapBox
            WrapBox_42->AddChild(ResourceWidget);
            ExchangeItems.Add(ResourceWidget);
        }
    }
}

void UMG_Exchange::SetResourceInfo(UUMG_Rs* ResourceWidget, const FResourceStruct& ResourceData, int32 ActorID, int32 Index)
{
    if (ResourceWidget)
    {
        // ���û�����Ϣ
        ResourceWidget->SetActorID(ActorID);
        ResourceWidget->SetIndex(Index);
        
        // ������Դ����
        ResourceWidget->FUN_SetRsInfo(ResourceData);
    }
}

// -------- ��������ʵ�� --------

APS_PLAY* UMG_Exchange::GetPlayerState() const
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    return PC ? Cast<APS_PLAY>(PC->PlayerState) : nullptr;
}

APC_PLAY* UMG_Exchange::GetPlayerController() const
{
    return Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
