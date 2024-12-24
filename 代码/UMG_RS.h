#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "UMG_RS.generated.h"

class APS_PLAY;
class APC_PLAY;
struct FResourceStruct;

UCLASS()
class YOURPROJECT_API UUMG_RS : public UUserWidget
{
    GENERATED_BODY()

public:
    UUMG_RS(const FObjectInitializer& ObjectInitializer);
    
    virtual void NativeConstruct() override;

    // ---------- 拖放功能 ----------
    /** 处理拖放事件 */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation);

    /** 处理拖动检测事件 */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    void OnDragDetected(FGeometry MyGeometry, const FPointerEvent& PointerEvent);

    /** 处理鼠标按下事件 */
    UFUNCTION(BlueprintCallable, Category = "Mouse Events")
    FReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

    // ---------- 资源管理 ----------
    /** 设置资源信息 */
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void FUN_SetRsInfo(const FResourceStruct& ResourceStruct);

    /** 获取资源数据 */
    UFUNCTION(BlueprintCallable, Category = "Resource")
    const FResourceStruct* GetResourceData() const { return CurrentResource; }

    // ---------- 属性设置 ----------
    /** 设置Actor ID */
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void SetActorID(int32 NewID) { ActorID = NewID; }

    /** 设置索引 */
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void SetIndex(int32 NewIndex) { Index = NewIndex; }

    /** 设置资源图标 */
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void SetResourceImage(UTexture2D* NewImage);

protected:
    // ---------- UI组件 ----------
    /** 资源图标 */
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Icon;

    /** 资源图片 */
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_169;

    /** 资源文本 */
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Resource;

    /** 价格文本 */
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Price;

    // ---------- 数据成员 ----------
    /** Actor ID */
    UPROPERTY(BlueprintReadOnly, Category = "Resource")
    int32 ActorID;

    /** 资源索引 */
    UPROPERTY(BlueprintReadOnly, Category = "Resource")
    int32 Index;

    /** 当前资源数据 */
    UPROPERTY()
    FResourceStruct* CurrentResource;

    // ---------- 辅助函数 ----------
    /** 获取玩家状态 */
    APS_PLAY* GetPlayerState() const;

    /** 获取玩家控制器 */
    APC_PLAY* GetPlayerController() const;

    /** 创建拖放操作 */
    UDragDropOperation* CreateDragDropOperation();

    /** 创建拖放视觉效果 */
    UUMG_RS* CreateDragVisual();

    /** 检查资源是否有效 */
    bool IsValid() const;

    /** 检测拖动操作 */
    FReply DetectDragOperation(const FPointerEvent& MouseEvent);

    /** 更新资源图标 */
    void UpdateResourceIcon(UTexture2D* IconTexture);

    /** 设置控件可见性 */
    void SetResourceVisibility(ESlateVisibility NewVisibility);

    /** 设置控件是否可用 */
    void SetResourceEnabled(bool bEnabled);

    /** 设置控件可见性 */
    void SetWidgetVisibility(UWidget* Widget, ESlateVisibility NewVisibility);
};
