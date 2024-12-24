#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_PLAY.generated.h"

// 前向声明
class UAcom_Vector;
class UUMGVector;
class UBPINRsNotify;
struct FResourceStruct;

// 委托声明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSold, int32, ItemID, int32, Price);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceAdded, AActor*, Build, int32, ID, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewAmount);

UCLASS()
class YOURPROJECT_API APS_PLAY : public APlayerState
{
    GENERATED_BODY()

public:
    APS_PLAY();

    // ---------- 组件引用 ----------
    /** Acom Vector组件 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAcom_Vector* Acom_Vector;

    /** UMG Vector组件 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UUMGVector* UMG_Vector;

    /** BPIN通知组件 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBPINRsNotify* BPINNotify;

    // ---------- 货币系统 ----------
    /** 初始金币数量 */
    UPROPERTY(EditDefaultsOnly, Category = "Currency")
    int32 InitialGold = 1000;

    /** 当前金币数量 */
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Currency")
    int32 Money;

    /** 服务器增加金钱 */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Currency")
    void FUN_Server_Money_Add(int32 Value);

    /** 服务器减少金钱 */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Currency")
    void FUN_Server_Money_Reduce(int32 Value);

    /** 获取当前金钱数量 */
    UFUNCTION(BlueprintPure, Category = "Currency")
    int32 GetMoney() const { return Money; }

    /** 检查是否有足够的金钱 */
    UFUNCTION(BlueprintPure, Category = "Currency")
    bool HasEnoughMoney(int32 Amount) const;

    // ---------- 拖放系统 ----------
    /** 服务器拖放事件 */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
    void EVE_Server_DragDrop(APlayerController* PC, AActor* TargetA, int32 AIndex, 
                            AActor* TargetB, int32 BIndex);

    /** 执行拖放操作 */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void FUN_Server_Drag_Drop(APS_PLAY* Target, AActor* TargetA, int32 AIndex, 
                             AActor* TargetB, int32 BIndex);

    // ---------- 资源管理 ----------
    /** 服务器添加资源到Build */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Resources")
    void EVE_Add_Rs_ToBuild(AActor* Build, int32 ID);

    /** 添加资源函数 */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool FUN_Add_Rs(AActor* Target, int32 ID, int32& OutIndex);

    /** 服务器卖出物品 */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Trading")
    void EVE_Sell_Rs(int32 Index, APlayerController* PC, UAcom_Vector* Vector);

    /** 获取资源结构体 */
    UFUNCTION(BlueprintPure, Category = "Resources")
    FResourceStruct* GetResourceStruct(UAcom_Vector* Vector, int32 Index) const;

    /** 移除资源 */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool FUN_Remove_Rs(AActor* Target, int32 RemoveIndex, UAcom_Vector* AcomVector);

    // ---------- UI更新 ----------
    /** 客户端更新UMG */
    UFUNCTION(Client, Reliable)
    void EVE_Client_Update_UMG(AActor* Target, int32 VectorID, AActor* Rs, int32 Index);

    /** 获取玩家状态 */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    static APS_PLAY* FUNL_Get_Playerstate_by_PC(APlayerController* PC);

    // ---------- 事件委托 ----------
    /** 物品售出事件 */
    UPROPERTY(BlueprintAssignable, Category = "Trading")
    FOnItemSold OnItemSold;

    /** 资源添加事件 */
    UPROPERTY(BlueprintAssignable, Category = "Resources")
    FOnResourceAdded OnResourceAdded;

    /** 金钱变化事件 */
    UPROPERTY(BlueprintAssignable, Category = "Currency")
    FOnMoneyChanged OnMoneyChanged;

protected:
    // ---------- 网络复制 ----------
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** 金钱数量变化回调 */
    UFUNCTION()
    virtual void OnRep_Money();

    // ---------- 内部辅助函数 ----------
    /** 设置金钱数量 */
    void SetMoney(int32 NewValue);

    /** 验证拖放操作 */
    bool ValidateDragDrop(AActor* TargetA, int32 AIndex, AActor* TargetB, int32 BIndex) const;

    /** 处理物品交换 */
    bool HandleItemSwap(UAcom_Vector* VectorA, int32 AIndex, UAcom_Vector* VectorB, int32 BIndex);

    /** 处理物品购买 */
    bool HandleItemPurchase(APlayerController* PC, UAcom_Vector* ShopVector, int32 ShopIndex, 
                           UAcom_Vector* InventoryVector, int32 InventoryIndex);

    /** 处理物品销售 */
    bool HandleItemSale(const FResourceStruct* Resource, APlayerController* PC, 
                       UAcom_Vector* Vector, int32 Index);

    /** 验证资源添加 */
    bool ValidateResourceAddition(AActor* Build, int32 ID) const;

    /** 获取Build的Acom Vector组件 */
    UAcom_Vector* GetBuildAcomVector(AActor* Build) const;
};
