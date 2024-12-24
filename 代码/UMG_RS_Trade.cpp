#include "UMG_RS_Trade.h"
#include "Components/Button.h"
#include "PS_PLAY.h"
#include "Kismet/GameplayStatics.h"
#include "ResourceStruct.h"

UMG_RS_Trade::UMG_RS_Trade(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 初始化成员变量
}

void UMG_RS_Trade::NativeConstruct()
{
    Super::NativeConstruct();

    // 绑定按钮事件
    BindButtonEvents();
}

// -------- 拖放功能实现 --------

bool UMG_RS_Trade::OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
    // 验证拖放操作
    if (!ValidateDropOperation(Operation))
    {
        return false;
    }

    // 获取拖放的资源
    UUMG_RS* DroppedResource = Cast<UUMG_RS>(Operation->Payload);
    if (!DroppedResource)
    {
        return false;
    }

    // 处理资源拖放
    return HandleResourceDrop(DroppedResource);
}

bool UMG_RS_Trade::ValidateDropOperation(UDragDropOperation* Operation) const
{
    // 检查操作是否有效
    if (!Operation || !Operation->Payload)
    {
        return false;
    }

    // 检查玩家状态
    if (!GetPlayerState())
    {
        return false;
    }

    return true;
}

bool UMG_RS_Trade::HandleResourceDrop(UUMG_RS* DroppedResource)
{
    // 获取玩家状态
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return false;
    }

    // 获取资源数据
    const FResourceStruct* ResourceData = DroppedResource->GetResourceData();
    if (!ResourceData)
    {
        return false;
    }

    // 处理资源拖放逻辑
    // 这里可以添加具体的交易逻辑
    return true;
}

// -------- 按钮功能实现 --------

void UMG_RS_Trade::BindButtonEvents()
{
    // 检查并绑定按钮点击事件
    if (Button_0)
    {
        Button_0->OnClicked.AddDynamic(this, &UMG_RS_Trade::OnBuyButtonClicked);
    }
}

void UMG_RS_Trade::OnBuyButtonClicked()
{
    // 检查资源数据是否有效
    if (!GetResourceData())
    {
        return;
    }

    // 执行购买操作
    ExecuteBuyOperation();
}

void UMG_RS_Trade::ExecuteBuyOperation()
{
    // 获取玩家状态
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return;
    }

    // 获取当前资源数据
    const FResourceStruct* ResourceData = GetResourceData();
    if (!ResourceData)
    {
        return;
    }

    // 调用服务器购买事件
    PlayerState->EVE_Buy_Rs(ResourceData);
}

// -------- 辅助功能实现 --------

APS_PLAY* UMG_RS_Trade::GetPlayerState() const
{
    // 获取玩家控制器
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        return nullptr;
    }

    // 返回玩家状态
    return Cast<APS_PLAY>(PC->PlayerState);
}
