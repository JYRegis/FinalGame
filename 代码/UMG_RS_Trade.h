#pragma once

#include "CoreMinimal.h"
#include "UMG_RS.h"
#include "Blueprint/DragDropOperation.h"
#include "UMG_RS_Trade.generated.h"

class APS_PLAY;

/**
 * UMG_RS_Trade类
 * 交易界面中的资源项目控件
 * 处理资源的显示、拖放和购买操作
 */
UCLASS()
class YOURPROJECT_API UUMG_RS_Trade : public UUMG_RS
{
    GENERATED_BODY()

public:
    /** 构造函数 */
    UMG_RS_Trade(const FObjectInitializer& ObjectInitializer);

    /** 处理拖放事件 */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    virtual bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation) override;

protected:
    /** 界面构造时调用 */
    virtual void NativeConstruct() override;

    // -------- UI组件 --------
    /** 购买按钮引用 */
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_0;

    // -------- 按钮事件 --------
    /** 购买按钮点击事件 */
    UFUNCTION()
    void OnBuyButtonClicked();

    /** 绑定按钮事件 */
    UFUNCTION()
    void BindButtonEvents();

    // -------- 交易功能 --------
    /** 执行购买操作 */
    UFUNCTION()
    void ExecuteBuyOperation();

    /** 验证拖放操作 */
    UFUNCTION()
    bool ValidateDropOperation(UDragDropOperation* Operation) const;

    // -------- 辅助功能 --------
    /** 获取玩家状态 */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

private:
    /** 资源拖放操作的结果处理 */
    bool HandleResourceDrop(UUMG_RS* DroppedResource);
};
