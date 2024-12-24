#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Bp.generated.h"

class UUMG_Vector;
class APS_PLAY;
class APC_PLAY;

/**
 * 建筑类型枚举
 * 用于标识不同类型的建筑物
 */
UENUM(BlueprintType)
enum class EBuildType : uint8
{
    None = 0,
    Exchange UMETA(DisplayName = "交易站"),
    // 在此添加其他建筑类型
};

/**
 * UMG_Bp类
 * 处理背包和建筑菜单的UI界面
 */
UCLASS()
class YOURPROJECT_API UUMG_Bp : public UUserWidget
{
    GENERATED_BODY()

public:
    /** 构造函数 */
    UMG_Bp(const FObjectInitializer& ObjectInitializer);

    /** 更新建筑菜单 */
    UFUNCTION(BlueprintCallable, Category = "Building")
    void FUN_Update_Open();

protected:
    /** 界面构造时调用 */
    virtual void NativeConstruct() override;

    // -------- UI组件引用 --------
    /** 背包Vector控件引用 */
    UPROPERTY(meta = (BindWidget))
    UUMG_Vector* Umg_Vector_Bp;

    /** 开放建造菜单Vector控件 */
    UPROPERTY(meta = (BindWidget))
    UUMG_Vector* Umg_Vector_Open;

    // -------- 定时器相关 --------
    /** 延迟更新定时器句柄 */
    FTimerHandle UpdateTimerHandle;

    /** 延迟时间 */
    const float DelayDuration = 0.2f;

    // -------- 延迟更新功能 --------
    /** 延迟更新函数 */
    UFUNCTION()
    void DelayedUpdate();

    /** 验证并更新背包 */
    UFUNCTION()
    void ValidateAndUpdate();

    // -------- 背包更新功能 --------
    /** 更新背包网格 */
    UFUNCTION()
    void UpdateBackpackGrid();

    /** 更新背包资源 */
    UFUNCTION()
    void UpdateBackpackResources();

    // -------- 建筑菜单功能 --------
    /** 获取建筑类型 */
    UFUNCTION()
    EBuildType GetBuildType() const;

    /** 设置建筑索引 */
    UFUNCTION()
    void SetBuildIndex(int32 Index);

    /** 更新建筑网格 */
    UFUNCTION()
    void UpdateBuildGrid(EBuildType BuildType);

    /** 更新建筑资源 */
    UFUNCTION()
    void UpdateBuildResources();

    // -------- 辅助功能 --------
    /** 获取玩家状态 */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

    /** 获取操作员Actor */
    UFUNCTION()
    AActor* GetOperatorActor() const;
};
