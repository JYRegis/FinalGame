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
    // 构造函数初始化
}

void UUMG_Vector::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 初始化时的额外设置
}

// ---------- 网格更新功能 ----------
void UUMG_Vector::FUN_Update_Grid(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID)
{
    // 清除现有内容
    ClearGrid();

    // 检查WrapBox是否有效
    if (!WrapBox_42)
    {
        return;
    }

    // 创建并添加新的UMG资源
    CreateAndAddUMGForResources(UMGRsArray, ActorID);
}

void UUMG_Vector::FUN_Update_Rs(const FResourceStruct& ResourceStruct, int32 Index)
{
    // 获取指定索引的UMG资源实例
    UUMG_Rs* TargetRs = GetUMGRsAtIndex(Index);
    if (!TargetRs)
    {
        return;
    }

    // 设置资源信息
    FUN_Set_Rs_Info(TargetRs, ResourceStruct);
}

void UUMG_Vector::FUN_Update_Rss(int32 ActorID)
{
    // 获取玩家控制器
    APC_PLAY* PC = GetPlayerController();
    if (!PC)
    {
        return;
    }

    // 获取对应Actor
    AActor* TargetActor = PC->GetActorWithID(ActorID);
    if (!TargetActor)
    {
        return;
    }

    // 获取资源数组
    TArray<FResourceStruct> Resources = GetResourcesFromActor(TargetActor);
    
    // 遍历并更新每个资源
    for (int32 Index = 0; Index < Resources.Num(); ++Index)
    {
        UUMG_Rs* TargetRs = GetUMGRsAtIndex(Index);
        if (TargetRs)
        {
            FUN_Set_Rs_Info(TargetRs, Resources[Index]);
        }
    }
}

// ---------- 辅助函数 ----------
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
