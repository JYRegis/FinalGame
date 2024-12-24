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
    // ��ʼ����Ա����
    ActorID = -1;
    Index = -1;
    CurrentResource = nullptr;
}

void UUMG_RS::NativeConstruct()
{
    Super::NativeConstruct();
    
    // ��ʼ��ʱ�Ķ�������
}

// ---------- �ϷŹ��� ----------
bool UUMG_RS::OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
    // ���Խ�Operationת��ΪUMG_RS
    UUMG_RS* DroppedItem = Cast<UUMG_RS>(Operation->Payload);
    if (!DroppedItem)
    {
        return false;
    }

    // ��ȡ���״̬
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return false;
    }

    // ��ȡ��ҿ�����
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return false;
    }

    // ��ȡԴ��Ŀ�����Ϣ
    int32 SourceActorID = DroppedItem->ActorID;
    int32 SourceIndex = DroppedItem->Index;
    int32 TargetActorID = ActorID;
    int32 TargetIndex = Index;

    // ���÷������Ϸ��¼�
    PlayerState->EVE_Server_DragDrop(PC, 
        PC->GetActorWithID(SourceActorID), SourceIndex,
        PC->GetActorWithID(TargetActorID), TargetIndex);

    return true;
}

void UUMG_RS::OnDragDetected(FGeometry MyGeometry, const FPointerEvent& PointerEvent)
{
    // �����ϷŲ���
    UDragDropOperation* DragDropOp = CreateDragDropOperation();
    if (!DragDropOp)
    {
        return;
    }

    // ��ʼ�ϷŲ���
    UWidgetBlueprintLibrary::DetectDragIfPressed(PointerEvent, this, EKeys::LeftMouseButton);
}

FReply UUMG_RS::OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
    // �����Դ�Ƿ���Ч
    if (!IsValid())
    {
        return FReply::Unhandled();
    }

    // �����������
    if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // ���Լ���϶�����
        return DetectDragOperation(MouseEvent);
    }

    return FReply::Unhandled();
}

// ---------- ��Դ���� ----------
void UUMG_RS::FUN_SetRsInfo(const FResourceStruct& ResourceStruct)
{
    // �����Դ�Ƿ���Ч
    if (!IsValid())
    {
        // �����Ч�����ػ������ؿؼ�
        SetResourceVisibility(ESlateVisibility::Collapsed);
        SetResourceEnabled(false);
        return;
    }

    // ������Դ����
    if (!CurrentResource)
    {
        CurrentResource = new FResourceStruct();
    }
    *CurrentResource = ResourceStruct;

    // ����ͼ��
    if (Image_Icon)
    {
        UpdateResourceIcon(ResourceStruct.ICON);
        Image_Icon->SetDesiredSizeOverride(FVector2D(32.0f, 32.0f));
    }

    // �����ı���ʾ
    if (Text_Resource)
    {
        Text_Resource->SetText(FText::FromString(ResourceStruct.Text));
    }

    // ���¼۸���ʾ
    if (Text_Price)
    {
        Text_Price->SetText(FText::AsNumber(ResourceStruct.Price));
    }

    // ���ÿؼ��ɼ���
    SetResourceVisibility(ESlateVisibility::Visible);
    SetResourceEnabled(true);
}

// ---------- �������� ----------
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
