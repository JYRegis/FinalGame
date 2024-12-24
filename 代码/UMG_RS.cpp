#include "UMG_RS.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PS_PLAY.h"
#include "PC_PLAY.h"
#include "ResourceStruct.h"
#include "Kismet/GameplayStatics.h"

UUMG_RS::UUMG_RS(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 初始化成员变量
    ActorID = -1;
    Index = -1;
    CurrentResource = nullptr;
}

void UUMG_RS::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 初始化时的额外设置
}

// ---------- 拖放功能 ----------
bool UUMG_RS::OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
    // 尝试将Operation转换为UMG_RS
    UUMG_RS* DroppedItem = Cast<UUMG_RS>(Operation->Payload);
    if (!DroppedItem)
    {
        return false;
    }

    // 获取玩家状态
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return false;
    }

    // 获取玩家控制器
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return false;
    }

    // 获取源和目标的信息
    int32 SourceActorID = DroppedItem->ActorID;
    int32 SourceIndex = DroppedItem->Index;
    int32 TargetActorID = ActorID;
    int32 TargetIndex = Index;

    // 调用服务器拖放事件
    PlayerState->EVE_Server_DragDrop(PC, 
        PC->GetActorWithID(SourceActorID), SourceIndex,
        PC->GetActorWithID(TargetActorID), TargetIndex);

    return true;
}

void UUMG_RS::OnDragDetected(FGeometry MyGeometry, const FPointerEvent& PointerEvent)
{
    // 创建拖放操作
    UDragDropOperation* DragDropOp = CreateDragDropOperation();
    if (!DragDropOp)
    {
        return;
    }

    // 开始拖放操作
    UWidgetBlueprintLibrary::DetectDragIfPressed(PointerEvent, this, EKeys::LeftMouseButton);
}

FReply UUMG_RS::OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
    // 检查资源是否有效
    if (!IsValid())
    {
        return FReply::Unhandled();
    }

    // 如果是鼠标左键
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // 尝试检测拖动操作
        return DetectDragOperation(MouseEvent);
    }

    return FReply::Unhandled();
}

// ---------- 资源管理 ----------
void UUMG_RS::FUN_SetRsInfo(const FResourceStruct& ResourceStruct)
{
    // 检查资源是否有效
    if (!IsValid())
    {
        // 如果无效，隐藏或禁用相关控件
        SetResourceVisibility(ESlateVisibility::Collapsed);
        SetResourceEnabled(false);
        return;
    }

    // 保存资源数据
    if (!CurrentResource)
    {
        CurrentResource = new FResourceStruct();
    }
    *CurrentResource = ResourceStruct;

    // 更新图标
    if (Image_Icon)
    {
        UpdateResourceIcon(ResourceStruct.ICON);
        Image_Icon->SetDesiredSizeOverride(FVector2D(32.0f, 32.0f));
    }

    // 更新文本显示
    if (Text_Resource)
    {
        Text_Resource->SetText(FText::FromString(ResourceStruct.Text));
    }

    // 更新价格显示
    if (Text_Price)
    {
        Text_Price->SetText(FText::AsNumber(ResourceStruct.Price));
    }

    // 设置控件可见性
    SetResourceVisibility(ESlateVisibility::Visible);
    SetResourceEnabled(true);
}

// ---------- 辅助函数 ----------
APS_PLAY* UUMG_RS::GetPlayerState() const
{
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return nullptr;
    }
    return Cast<APS_PLAY>(PC->PlayerState);
}

APC_PLAY* UUMG_RS::GetPlayerController() const
{
    return Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

UDragDropOperation* UUMG_RS::CreateDragDropOperation()
{
    UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
    if (!DragDropOp)
    {
        return nullptr;
    }

    UUMG_RS* DragVisual = CreateDragVisual();
    if (!DragVisual)
    {
        return nullptr;
    }

    DragDropOp->Payload = this;
    DragDropOp->DefaultDragVisual = DragVisual;
    DragDropOp->Pivot = EDragPivot::CenterCenter;
    DragDropOp->Offset = FVector2D(0.0f, 0.0f);

    SetWidgetVisibility(this, ESlateVisibility::Visible);

    return DragDropOp;
}

UUMG_RS* UUMG_RS::CreateDragVisual()
{
    UUMG_RS* DragVisual = CreateWidget<UUMG_RS>(GetOwningPlayer(), GetClass());
    if (!DragVisual)
    {
        return nullptr;
    }

    DragVisual->SetActorID(ActorID);
    DragVisual->SetIndex(Index);
    
    if (Image_169 && Image_169->GetBrushResource())
    {
        DragVisual->SetResourceImage(Cast<UTexture2D>(Image_169->GetBrushResource()));
    }

    return DragVisual;
}

bool UUMG_RS::IsValid() const
{
    return CurrentResource != nullptr;
}

FReply UUMG_RS::DetectDragOperation(const FPointerEvent& MouseEvent)
{
    FReply Reply = FReply::Handled();
    Reply.DetectDrag(this, EKeys::LeftMouseButton);
    return Reply;
}

void UUMG_RS::UpdateResourceIcon(UTexture2D* IconTexture)
{
    if (Image_Icon && IconTexture)
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(IconTexture);
        Image_Icon->SetBrush(Brush);
    }
}

void UUMG_RS::SetResourceImage(UTexture2D* NewImage)
{
    if (Image_169 && NewImage)
    {
        Image_169->SetBrushFromTexture(NewImage);
    }
}

void UUMG_RS::SetResourceVisibility(ESlateVisibility NewVisibility)
{
    if (Image_Icon)
    {
        Image_Icon->SetVisibility(NewVisibility);
    }
    if (Text_Resource)
    {
        Text_Resource->SetVisibility(NewVisibility);
    }
    if (Text_Price)
    {
        Text_Price->SetVisibility(NewVisibility);
    }
}

void UUMG_RS::SetResourceEnabled(bool bEnabled)
{
    if (Image_Icon)
    {
        Image_Icon->SetIsEnabled(bEnabled);
    }
    if (Text_Resource)
    {
        Text_Resource->SetIsEnabled(bEnabled);
    }
    if (Text_Price)
    {
        Text_Price->SetIsEnabled(bEnabled);
    }
}

void UUMG_RS::SetWidgetVisibility(UWidget* Widget, ESlateVisibility NewVisibility)
{
    if (Widget)
    {
        Widget->SetVisibility(NewVisibility);
    }
}
