#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "UMG_Exchange.generated.h"

class APS_PLAY;
class APC_PLAY;
class UUMG_Rs;

/**
 * UMG_Exchange类
 * 交易站界面控件
 * 处理资源交易、拖放操作和界面更新
 */
UCLASS()
class YOURPROJECT_API UUMG_Exchange : public UUserWidget
{
    GENERATED_BODY()

public:
    /** 构造函数 */
    UMG_Exchange(const FObjectInitializer& ObjectInitializer);

    /** 界面构造时调用 */
    virtual void NativeConstruct() override;

    /** 处理拖放事件 */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation);

    /** 更新交易站界面 */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void FUN_Update_Exchange();

protected:
    // -------- UI组件 --------
    /** 交易站资源列表 */
    UPROPERTY()
    TArray<UUMG_Rs*> ExchangeItems;

    /** 包装盒引用 - 用于存储资源项目 */
    UPROPERTY(meta = (BindWidget))
    class UWrapBox* WrapBox_42;

    // -------- 拖放功能 --------
    /** 验证拖放操作 */
    UFUNCTION()
    bool ValidateDropOperation(UDragDropOperation* Operation, UUMG_Rs*& OutResource) const;

    /** 处理资源出售 */
    UFUNCTION()
    bool HandleResourceSell(UUMG_Rs* DroppedResource, int32 ActorID, int32 Index);

    // -------- 更新功能 --------
    /** 清理现有资源项目 */
    UFUNCTION()
    void ClearExistingItems();

    /** 创建新的资源项目 */
    UFUNCTION()
    void CreateResourceItems(const TArray<FResourceStruct>& Resources, int32 ActorID);

    /** 设置资源信息 */
    UFUNCTION()
    void SetResourceInfo(UUMG_Rs* ResourceWidget, const FResourceStruct& ResourceData, int32 ActorID, int32 Index);

    // -------- 辅助功能 --------
    /** 获取玩家状态 */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

    /** 获取玩家控制器 */
    UFUNCTION()
    APC_PLAY* GetPlayerController() const;

private:
    /** 资源项目类 */
    UPROPERTY()
    TSubclassOf<UUMG_Rs> ResourceItemClass;
};
