#include "UMG_Bp.h"
#include "UMG_Vector.h"
#include "PS_PLAY.h"
#include "PC_PLAY.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// ���캯��
UMG_Bp::UMG_Bp(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // ��ʼ����Ա����
}

// ���湹��
void UMG_Bp::NativeConstruct()
{
    Super::NativeConstruct();

    // �����ӳٸ��¶�ʱ��
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            UpdateTimerHandle,
            this,
            &UMG_Bp::DelayedUpdate,
            DelayDuration,
            false  // ��ѭ��
        );
    }
}

// -------- �ӳٸ��¹���ʵ�� --------

void UMG_Bp::DelayedUpdate()
{
    // ִ����֤�͸���
    ValidateAndUpdate();
}

void UMG_Bp::ValidateAndUpdate()
{
    // ��ȡ���״̬
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return;
    }

    // ��֤����Vector���
    if (!Umg_Vector_Bp)
    {
        return;
    }

    // ���±�������
    UpdateBackpackGrid();
    UpdateBackpackResources();
}

// -------- �������¹���ʵ�� --------

void UMG_Bp::UpdateBackpackGrid()
{
    if (Umg_Vector_Bp)
    {
        // ��ȡ������Ʒ�б�
        TArray<UUMG_Rs*> BackpackItems;  // ��Ҫʵ�ֻ�ȡ������Ʒ���߼�
        int32 ActorID = 0;  // ���ú��ʵ�ActorID
        
        // ���±���������ʾ
        Umg_Vector_Bp->FUN_Update_Grid(BackpackItems, ActorID);
    }
}

void UMG_Bp::UpdateBackpackResources()
{
    if (Umg_Vector_Bp)
    {
        // ���±�����Դ��ʾ
        int32 ActorID = 0;  // ���ú��ʵ�ActorID
        Umg_Vector_Bp->FUN_Update_Rss(ActorID);
    }
}

// -------- �����˵�����ʵ�� --------

void UMG_Bp::FUN_Update_Open()
{
    // ��ȡ��ҿ�����
    APC_PLAY* PC = Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC)
    {
        return;
    }

    // ��ȡ����ԱActor
    AActor* Operator = GetOperatorActor();
    if (!Operator)
    {
        return;
    }

    // ��֤�����˵�Vector���
    if (!Umg_Vector_Open)
    {
        return;
    }

    // ��ȡ����������Ϣ
    EBuildType BuildType = GetBuildType();
    SetBuildIndex(0); // ����Ĭ������

    // ���½����˵���ʾ
    UpdateBuildGrid(BuildType);
    UpdateBuildResources();
}

EBuildType UMG_Bp::GetBuildType() const
{
    // ��ȡ��ǰ��������
    // ��Ҫ����ʵ����Ϸ�߼�ʵ��
    return EBuildType::Exchange;
}

void UMG_Bp::SetBuildIndex(int32 Index)
{
    // ���ý�������
    // ��Ҫ����ʵ������ʵ�ִ洢�߼�
}

void UMG_Bp::UpdateBuildGrid(EBuildType BuildType)
{
    if (Umg_Vector_Open)
    {
        // ���ݽ������ͻ�ȡ������Ŀ�б�
        TArray<UUMG_Rs*> BuildItems;  // ��Ҫ����BuildType���
        int32 ActorID = 0;  // ���ú��ʵ�ActorID
        
        // ���½���������ʾ
        Umg_Vector_Open->FUN_Update_Grid(BuildItems, ActorID);
    }
}

void UMG_Bp::UpdateBuildResources()
{
    if (Umg_Vector_Open)
    {
        // ���½�����Դ��ʾ
        int32 ActorID = 0;  // ���ú��ʵ�ActorID
        Umg_Vector_Open->FUN_Update_Rss(ActorID);
    }
}

// -------- ��������ʵ�� --------

APS_PLAY* UMG_Bp::GetPlayerState() const
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

AActor* UMG_Bp::GetOperatorActor() const
{
    // ��ȡ��ҿ�����
    APC_PLAY* PC = Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC)
    {
        return nullptr;
    }

    // ��ȡ����ԱActor
    return PC->GetOperatorActor();
}
