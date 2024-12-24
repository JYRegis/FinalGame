#include "UMG_RS_Trade.h"
#include "Components/Button.h"
#include "PS_PLAY.h"
#include "Kismet/GameplayStatics.h"
#include "ResourceStruct.h"

UMG_RS_Trade::UMG_RS_Trade(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // ��ʼ����Ա����
}

void UMG_RS_Trade::NativeConstruct()
{
    Super::NativeConstruct();

    // �󶨰�ť�¼�
    BindButtonEvents();
}

// -------- �ϷŹ���ʵ�� --------

bool UMG_RS_Trade::OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
    // ��֤�ϷŲ���
    if (!ValidateDropOperation(Operation))
    {
        return false;
    }

    // ��ȡ�Ϸŵ���Դ
    UUMG_RS* DroppedResource = Cast<UUMG_RS>(Operation->Payload);
    if (!DroppedResource)
    {
        return false;
    }

    // ������Դ�Ϸ�
    return HandleResourceDrop(DroppedResource);
}

bool UMG_RS_Trade::ValidateDropOperation(UDragDropOperation* Operation) const
{
    // �������Ƿ���Ч
    if (!Operation || !Operation->Payload)
    {
        return false;
    }

    // ������״̬
    if (!GetPlayerState())
    {
        return false;
    }

    return true;
}

bool UMG_RS_Trade::HandleResourceDrop(UUMG_RS* DroppedResource)
{
    // ��ȡ���״̬
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return false;
    }

    // ��ȡ��Դ����
    const FResourceStruct* ResourceData = DroppedResource->GetResourceData();
    if (!ResourceData)
    {
        return false;
    }

    // ������Դ�Ϸ��߼�
    // ���������Ӿ���Ľ����߼�
    return true;
}

// -------- ��ť����ʵ�� --------

void UMG_RS_Trade::BindButtonEvents()
{
    // ��鲢�󶨰�ť����¼�
    if (Button_0)
    {
        Button_0->OnClicked.AddDynamic(this, &UMG_RS_Trade::OnBuyButtonClicked);
    }
}

void UMG_RS_Trade::OnBuyButtonClicked()
{
    // �����Դ�����Ƿ���Ч
    if (!GetResourceData())
    {
        return;
    }

    // ִ�й������
    ExecuteBuyOperation();
}

void UMG_RS_Trade::ExecuteBuyOperation()
{
    // ��ȡ���״̬
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return;
    }

    // ��ȡ��ǰ��Դ����
    const FResourceStruct* ResourceData = GetResourceData();
    if (!ResourceData)
    {
        return;
    }

    // ���÷����������¼�
    PlayerState->EVE_Buy_Rs(ResourceData);
}

// -------- ��������ʵ�� --------

APS_PLAY* UMG_RS_Trade::GetPlayerState() const
{
    // ��ȡ��ҿ�����
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        return nullptr;
    }

    // �������״̬
    return Cast<APS_PLAY>(PC->PlayerState);
}
