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
    // 加载资源项目类蓝图
    static ConstructorHelpers::FClassFinder<UUMG_Rs> ResourceItemBPClass(TEXT("/Game/UI/WBP_RS"));
    if (ResourceItemBPClass.Succeeded())
    {
        ResourceItemClass = ResourceItemBPClass.Class;
    }
}

void UMG_Exchange::NativeConstruct()
{
    Super::NativeConstruct();

    // 初始化时更新交易站
    FUN_Update_Exchange();
}

// -------- 拖放功能实现 --------

bool UMG_Exchange::OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
    // 验证拖放的资源
    UUMG_Rs* DroppedResource = nullptr;
    if (!ValidateDropOperation(Operation, DroppedResource))
    {
        return false;
    }

    // 获取资源的ActorID和Index
    int32 ActorID = DroppedResource->GetActorID();
    int32 Index = DroppedResource->GetIndex();

    // 处理资源出售
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

    // 调用服务器出售事件
    PlayerState->EVE_Sell_Rs(PC, ActorID, Index, PC->GetVector());
    return true;
}

// -------- 更新功能实现 --------

void UMG_Exchange::FUN_Update_Exchange()
{
    // 获取玩家控制器
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return;
    }

    // 获取Actor ID
    int32 ActorID = PC->GetActorID();

    // 清理现有项目
    ClearExistingItems();

    // 获取资源列表
    TArray<FResourceStruct> Resources;
    if (APS_PLAY* PlayerState = GetPlayerState())
    {
        Resources = PlayerState->GetExchangeResources(ActorID);
    }

    // 创建新的资源项目
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

    // 为每个资源创建UI项目
    for (int32 Index = 0; Index < Resources.Num(); ++Index)
    {
        // 创建资源控件
        UUMG_Rs* ResourceWidget = CreateWidget<UUMG_Rs>(this, ResourceItemClass);
        if (ResourceWidget)
        {
            // 设置资源信息
            SetResourceInfo(ResourceWidget, Resources[Index], ActorID, Index);

            // 添加到WrapBox
            WrapBox_42->AddChild(ResourceWidget);
            ExchangeItems.Add(ResourceWidget);
        }
    }
}

void UMG_Exchange::SetResourceInfo(UUMG_Rs* ResourceWidget, const FResourceStruct& ResourceData, int32 ActorID, int32 Index)
{
    if (ResourceWidget)
    {
        // 设置基本信息
        ResourceWidget->SetActorID(ActorID);
        ResourceWidget->SetIndex(Index);
        
        // 设置资源数据
        ResourceWidget->FUN_SetRsInfo(ResourceData);
    }
}

// -------- 辅助功能实现 --------

APS_PLAY* UMG_Exchange::GetPlayerState() const
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    return PC ? Cast<APS_PLAY>(PC->PlayerState) : nullptr;
}

APC_PLAY* UMG_Exchange::GetPlayerController() const
{
    return Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}
