#include "UMG_Vector.h"
#include "Components/WrapBox.h"
#include "UMG_Rs.h"
#include "PC_PLAY.h"
#include "Acom_Vector.h"
#include "ResourceStruct.h"
#include "Kismet/GameplayStatics.h"

UUMG_Vector::UUMG_Vector(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // ���캯����ʼ��
}

void UUMG_Vector::NativeConstruct()
{
    Super::NativeConstruct();
    
    // ��ʼ��ʱ�Ķ�������
}

// ---------- ������¹��� ----------
void UUMG_Vector::FUN_Update_Grid(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID)
{
    // �����������
    ClearGrid();

    // ���WrapBox�Ƿ���Ч
    if (!WrapBox_42)
    {
        return;
    }

    // ����������µ�UMG��Դ
    CreateAndAddUMGForResources(UMGRsArray, ActorID);
}

void UUMG_Vector::FUN_Update_Rs(const FResourceStruct& ResourceStruct, int32 Index)
{
    // ��ȡָ��������UMG��Դʵ��
    UUMG_Rs* TargetRs = GetUMGRsAtIndex(Index);
    if (!TargetRs)
    {
        return;
    }

    // ������Դ��Ϣ
    FUN_Set_Rs_Info(TargetRs, ResourceStruct);
}

void UUMG_Vector::FUN_Update_Rss(int32 ActorID)
{
    // ��ȡ��ҿ�����
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return;
    }

    // ��ȡ��ӦActor
    AActor* TargetActor = PC->GetActorWithID(ActorID);
    if (!TargetActor)
    {
        return;
    }

    // ��ȡ��Դ����
    TArray<FResourceStruct> Resources = GetResourcesFromActor(TargetActor);
    
    // ����������ÿ����Դ
    for (int32 Index = 0; Index < Resources.Num(); ++Index)
    {
        UUMG_Rs* TargetRs = GetUMGRsAtIndex(Index);
        if (TargetRs)
        {
            FUN_Set_Rs_Info(TargetRs, Resources[Index]);
        }
    }
}

// ---------- �������� ----------
void UUMG_Vector::ClearGrid()
{
    if (WrapBox_42)
    {
        WrapBox_42->ClearChildren();
    }
    UMG_Rss.Empty();
}

APC_PLAY* UUMG_Vector::GetPlayerController() const
{
    return Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UUMG_Vector::CreateAndAddUMGForResources(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID)
{
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return;
    }

    for (int32 Index = 0; Index < UMGRsArray.Num(); ++Index)
    {
        UUMG_Rs* Rs = UMGRsArray[Index];
        if (Rs)
        {
            UUMG_Rs* NewRs = CreateWidget<UUMG_Rs>(GetWorld(), Rs->GetClass());
            if (NewRs)
            {
                NewRs->SetOwningPlayer(PC);
                NewRs->SetActorID(ActorID);
                NewRs->SetIndex(Index);
                
                if (Rs->GetResourceData())
                {
                    NewRs->InitializeFromResource(Rs->GetResourceData());
                }

                WrapBox_42->AddChild(NewRs);
                UMG_Rss.Add(NewRs);
            }
        }
    }
}

UUMG_Rs* UUMG_Vector::GetUMGRsAtIndex(int32 Index) const
{
    if (!UMG_Rss.IsValidIndex(Index))
    {
        return nullptr;
    }
    return UMG_Rss[Index];
}

void UUMG_Vector::FUN_Set_Rs_Info(UUMG_Rs* TargetRs, const FResourceStruct& ResourceStruct)
{
    if (!TargetRs)
    {
        return;
    }
    TargetRs->SetResourceInfo(ResourceStruct);
}

TArray<FResourceStruct> UUMG_Vector::GetResourcesFromActor(AActor* Actor) const
{
    TArray<FResourceStruct> Resources;
    
    if (!Actor)
    {
        return Resources;
    }

    UAcom_Vector* VectorComp = GetVectorAcom(Actor);
    if (VectorComp)
    {
        Resources = VectorComp->GetAllResources();
    }
    
    return Resources;
}

UAcom_Vector* UUMG_Vector::GetVectorAcom(AActor* Actor) const
{
    if (!Actor)
    {
        return nullptr;
    }
    return Actor->FindComponentByClass<UAcom_Vector>();
}
