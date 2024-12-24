#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Vector.generated.h"

class UUMG_Rs;
class APC_PLAY;
class FResourceStruct;
class UAcom_Vector;

UCLASS()
class YOURPROJECT_API UUMG_Vector : public UUserWidget
{
    GENERATED_BODY()

public:
    UUMG_Vector(const FObjectInitializer& ObjectInitializer);
    
    virtual void NativeConstruct() override;

    // ---------- 网格更新功能 ----------
    /** 更新网格函数 */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void FUN_Update_Grid(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID);

    /** 更新特定资源的信息 */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void FUN_Update_Rs(const FResourceStruct& ResourceStruct, int32 Index);

    /** 批量更新所有资源 */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void FUN_Update_Rss(int32 ActorID);

protected:
    // ---------- UI组件 ----------
    /** WrapBox容器引用 */
    UPROPERTY(meta = (BindWidget))
    class UWrapBox* WrapBox_42;

    /** 存储UMG资源实例的数组 */
    UPROPERTY()
    TArray<UUMG_Rs*> UMG_Rss;

    // ---------- 辅助函数 ----------
    /** 清除网格内容 */
    void ClearGrid();

    /** 获取玩家控制器 */
    APC_PLAY* GetPlayerController() const;

    /** 为每个资源创建并添加UMG */
    void CreateAndAddUMGForResources(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID);

    /** 根据索引获取UMG资源实例 */
    UUMG_Rs* GetUMGRsAtIndex(int32 Index) const;

    /** 设置资源信息 */
    void FUN_Set_Rs_Info(UUMG_Rs* TargetRs, const FResourceStruct& ResourceStruct);

    /** 获取Vector组件中的资源数组 */
    TArray<FResourceStruct> GetResourcesFromActor(AActor* Actor) const;

    /** 获取Actor的Vector组件 */
    UAcom_Vector* GetVectorAcom(AActor* Actor) const;
};
