#include "UMG_Bp.h"
#include "UMG_Vector.h"
#include "PS_PLAY.h"
#include "PC_PLAY.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// 构造函数
UMG_Bp::UMG_Bp(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 初始化成员变量
}

// 界面构造
void UMG_Bp::NativeConstruct()
{
    Super::NativeConstruct();

    // 设置延迟更新定时器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            UpdateTimerHandle,
            this,
            &UMG_Bp::DelayedUpdate,
            DelayDuration,
            false  // 不循环
        );
    }
}

// -------- 延迟更新功能实现 --------

void UMG_Bp::DelayedUpdate()
{
    // 执行验证和更新
    ValidateAndUpdate();
}

void UMG_Bp::ValidateAndUpdate()
{
    // 获取玩家状态
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        return;
    }

    // 验证背包Vector组件
    if (!Umg_Vector_Bp)
    {
        return;
    }

    // 更新背包内容
    UpdateBackpackGrid();
    UpdateBackpackResources();
}

// -------- 背包更新功能实现 --------

void UMG_Bp::UpdateBackpackGrid()
{
    if (Umg_Vector_Bp)
    {
        // 获取背包物品列表
        TArray<UUMG_Rs*> BackpackItems;  // 需要实现获取背包物品的逻辑
        int32 ActorID = 0;  // 设置合适的ActorID
        
        // 更新背包网格显示
        Umg_Vector_Bp->FUN_Update_Grid(BackpackItems, ActorID);
    }
}

void UMG_Bp::UpdateBackpackResources()
{
    if (Umg_Vector_Bp)
    {
        // 更新背包资源显示
        int32 ActorID = 0;  // 设置合适的ActorID
        Umg_Vector_Bp->FUN_Update_Rss(ActorID);
    }
}

// -------- 建筑菜单功能实现 --------

void UMG_Bp::FUN_Update_Open()
{
    // 获取玩家控制器
    APC_PLAY* PC = Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC)
    {
        return;
    }

    // 获取操作员Actor
    AActor* Operator = GetOperatorActor();
    if (!Operator)
    {
        return;
    }

    // 验证建筑菜单Vector组件
    if (!Umg_Vector_Open)
    {
        return;
    }

    // 获取并处理建筑信息
    EBuildType BuildType = GetBuildType();
    SetBuildIndex(0); // 设置默认索引

    // 更新建筑菜单显示
    UpdateBuildGrid(BuildType);
    UpdateBuildResources();
}

EBuildType UMG_Bp::GetBuildType() const
{
    // 获取当前建筑类型
    // 需要根据实际游戏逻辑实现
    return EBuildType::Exchange;
}

void UMG_Bp::SetBuildIndex(int32 Index)
{
    // 设置建筑索引
    // 需要根据实际需求实现存储逻辑
}

void UMG_Bp::UpdateBuildGrid(EBuildType BuildType)
{
    if (Umg_Vector_Open)
    {
        // 根据建筑类型获取建筑项目列表
        TArray<UUMG_Rs*> BuildItems;  // 需要根据BuildType填充
        int32 ActorID = 0;  // 设置合适的ActorID
        
        // 更新建筑网格显示
        Umg_Vector_Open->FUN_Update_Grid(BuildItems, ActorID);
    }
}

void UMG_Bp::UpdateBuildResources()
{
    if (Umg_Vector_Open)
    {
        // 更新建筑资源显示
        int32 ActorID = 0;  // 设置合适的ActorID
        Umg_Vector_Open->FUN_Update_Rss(ActorID);
    }
}

// -------- 辅助功能实现 --------

APS_PLAY* UMG_Bp::GetPlayerState() const
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

AActor* UMG_Bp::GetOperatorActor() const
{
    // 获取玩家控制器
    APC_PLAY* PC = Cast<APC_PLAY>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC)
    {
        return nullptr;
    }

    // 获取操作员Actor
    return PC->GetOperatorActor();
}
