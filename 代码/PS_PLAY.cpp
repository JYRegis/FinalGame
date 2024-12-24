#include "PS_PLAY.h"
#include "Acom_Vector.h"
#include "UMGVector.h"
#include "BPINRsNotify.h"
#include "ResourceStruct.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"

APS_PLAY::APS_PLAY()
{
    // 创建必要的组件
    Acom_Vector = CreateDefaultSubobject<UAcom_Vector>(TEXT("Acom_Vector"));
    UMG_Vector = CreateDefaultSubobject<UUMGVector>(TEXT("UMG_Vector"));
    BPINNotify = CreateDefaultSubobject<UBPINRsNotify>(TEXT("BPINNotify"));

    // 设置初始金钱
    Money = InitialGold;
}

// ---------- 网络复制 ----------
void APS_PLAY::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 注册Money变量为可复制，并使用OnRep_Money作为回调
    DOREPLIFETIME_CONDITION_NOTIFY(APS_PLAY, Money, COND_None, REPNOTIFY_OnChanged);
}

// ---------- 货币系统 ----------
void APS_PLAY::FUN_Server_Money_Add_Implementation(int32 Value)
{
    // 确保在服务器上执行
    if (!HasAuthority())
    {
        return;
    }

    // 获取当前金钱数量并增加
    int32 CurrentMoney = GetMoney();
    int32 NewMoney = CurrentMoney + Value;
    
    // 设置新的金钱数量
    SetMoney(NewMoney);
}

void APS_PLAY::FUN_Server_Money_Reduce_Implementation(int32 Value)
{
    // 确保在服务器上执行
    if (!HasAuthority())
    {
        return;
    }

    // 检查是否有足够的金钱
    if (HasEnoughMoney(Value))
    {
        // 计算新的金钱数量
        int32 CurrentMoney = GetMoney();
        int32 NewMoney = CurrentMoney - Value;
        
        // 设置新的金钱数量
        SetMoney(NewMoney);
        
        // 返回成功
        OnMoneyChanged.Broadcast(NewMoney);
    }
}

void APS_PLAY::SetMoney(int32 NewValue)
{
    // 确保金钱不会为负数
    NewValue = FMath::Max(0, NewValue);
    
    // 如果金钱数量发生变化
    if (Money != NewValue)
    {
        Money = NewValue;
        
        // 如果在服务器上，这个变化会自动复制到客户端
        if (HasAuthority())
        {
            OnMoneyChanged.Broadcast(Money);
        }
        
        // 触发金钱变化的回调
        OnRep_Money();
    }
}

void APS_PLAY::OnRep_Money()
{
    // 在这里处理金钱变化后的UI更新等逻辑
    // 例如触发委托或事件来更新UI
    OnMoneyChanged.Broadcast(Money);
}

bool APS_PLAY::HasEnoughMoney(int32 Amount) const
{
    return Money >= Amount;
}

// ---------- 拖放系统 ----------
void APS_PLAY::EVE_Server_DragDrop_Implementation(APlayerController* PC, AActor* TargetA, 
    int32 AIndex, AActor* TargetB, int32 BIndex)
{
    // 确保在服务器上执行
    if (!HasAuthority())
    {
        return;
    }

    // 验证参数
    if (!ValidateDragDrop(TargetA, AIndex, TargetB, BIndex))
    {
        return;
    }

    // 执行拖放操作
    FUN_Server_Drag_Drop(this, TargetA, AIndex, TargetB, BIndex);
}

void APS_PLAY::FUN_Server_Drag_Drop(APS_PLAY* Target, AActor* TargetA, int32 AIndex, 
    AActor* TargetB, int32 BIndex)
{
    // 获取两个目标的Acom Vector组件
    UAcom_Vector* VectorA = TargetA->FindComponentByClass<UAcom_Vector>();
    UAcom_Vector* VectorB = TargetB->FindComponentByClass<UAcom_Vector>();

    if (!VectorA || !VectorB)
    {
        return;
    }

    // 检查是否是商店交易
    bool bIsShopTransaction = VectorA->IsShop() || VectorB->IsShop();
    
    if (bIsShopTransaction)
    {
        // 处理购买逻辑
        UAcom_Vector* ShopVector = VectorA->IsShop() ? VectorA : VectorB;
        UAcom_Vector* InventoryVector = VectorA->IsShop() ? VectorB : VectorA;
        int32 ShopIndex = VectorA->IsShop() ? AIndex : BIndex;
        int32 InventoryIndex = VectorA->IsShop() ? BIndex : AIndex;

        if (HandleItemPurchase(Target->GetOwningController(), ShopVector, ShopIndex, 
                             InventoryVector, InventoryIndex))
        {
            // 购买成功，更新UI
            EVE_Client_Update_UMG(TargetA, VectorA->GetVectorID(), TargetA, AIndex);
            EVE_Client_Update_UMG(TargetB, VectorB->GetVectorID(), TargetB, BIndex);
        }
    }
    else
    {
        // 处理普通物品交换
        if (HandleItemSwap(VectorA, AIndex, VectorB, BIndex))
        {
            // 交换成功，更新UI
            EVE_Client_Update_UMG(TargetA, VectorA->GetVectorID(), TargetA, AIndex);
            EVE_Client_Update_UMG(TargetB, VectorB->GetVectorID(), TargetB, BIndex);
        }
    }
}

bool APS_PLAY::ValidateDragDrop(AActor* TargetA, int32 AIndex, AActor* TargetB, int32 BIndex) const
{
    // 基本验证
    if (!TargetA || !TargetB || AIndex < 0 || BIndex < 0)
    {
        return false;
    }

    // 检查组件
    UAcom_Vector* VectorA = TargetA->FindComponentByClass<UAcom_Vector>();
    UAcom_Vector* VectorB = TargetB->FindComponentByClass<UAcom_Vector>();

    if (!VectorA || !VectorB)
    {
        return false;
    }

    // 检查索引是否有效
    if (!VectorA->IsValidIndex(AIndex) || !VectorB->IsValidIndex(BIndex))
    {
        return false;
    }

    return true;
}

bool APS_PLAY::HandleItemSwap(UAcom_Vector* VectorA, int32 AIndex, UAcom_Vector* VectorB, int32 BIndex)
{
    if (!VectorA || !VectorB)
    {
        return false;
    }

    // 获取物品数据
    FResourceStruct* ItemA = GetResourceStruct(VectorA, AIndex);
    FResourceStruct* ItemB = GetResourceStruct(VectorB, BIndex);

    if (!ItemA || !ItemB)
    {
        return false;
    }

    // 执行交换
    VectorA->SwapResources(AIndex, VectorB, BIndex);

    return true;
}

bool APS_PLAY::HandleItemPurchase(APlayerController* PC, UAcom_Vector* ShopVector, int32 ShopIndex, 
    UAcom_Vector* InventoryVector, int32 InventoryIndex)
{
    if (!PC || !ShopVector || !InventoryVector)
    {
        return false;
    }

    // 获取商品信息
    FResourceStruct* ShopItem = GetResourceStruct(ShopVector, ShopIndex);
    if (!ShopItem)
    {
        return false;
    }

    // 检查玩家是否有足够的金钱
    if (!HasEnoughMoney(ShopItem->Price))
    {
        // 可以在这里添加提示
        return false;
    }

    // 扣除金钱
    FUN_Server_Money_Reduce(ShopItem->Price);

    // 转移物品
    return InventoryVector->AddResource(ShopItem->ID, InventoryIndex);
}
// ---------- 资源管理 ----------
void APS_PLAY::EVE_Add_Rs_ToBuild_Implementation(AActor* Build, int32 ID)
{
    // 确保在服务器上执行
    if (!HasAuthority())
    {
        return;
    }

    // 验证参数
    if (!ValidateResourceAddition(Build, ID))
    {
        return;
    }

    // 获取Acom Vector组件
    UAcom_Vector* Vector = GetBuildAcomVector(Build);
    if (!Vector)
    {
        return;
    }

    // 添加资源
    int32 Index;
    if (FUN_Add_Rs(Build, ID, Index))
    {
        // 触发资源添加事件
        OnResourceAdded.Broadcast(Build, ID, Index);
    }
}

bool APS_PLAY::FUN_Add_Rs(AActor* Target, int32 ID, int32& OutIndex)
{
    if (!Target)
    {
        return false;
    }

    // 获取Acom Vector组件
    UAcom_Vector* Vector = GetBuildAcomVector(Target);
    if (!Vector)
    {
        return false;
    }

    // 获取下一个可用索引
    OutIndex = Vector->GetNextAvailableIndex();
    if (OutIndex == -1)
    {
        // 没有可用位置
        return false;
    }

    // 添加资源
    bool bSuccess = Vector->AddResource(ID, OutIndex);
    if (bSuccess)
    {
        // 更新客户端UI
        EVE_Client_Update_UMG(Target, Vector->GetVectorID(), Target, OutIndex);
    }

    return bSuccess;
}

void APS_PLAY::EVE_Sell_Rs_Implementation(int32 Index, APlayerController* PC, UAcom_Vector* Vector)
{
    // 确保在服务器上执行
    if (!HasAuthority())
    {
        return;
    }

    // 获取资源结构体
    FResourceStruct* Resource = GetResourceStruct(Vector, Index);
    if (!Resource)
    {
        return;
    }

    // 处理物品销售
    if (HandleItemSale(Resource, PC, Vector, Index))
    {
        // 销售成功，移除物品
        AActor* Target = Vector->GetOwner();
        FUN_Remove_Rs(Target, Index, Vector);
    }
}

FResourceStruct* APS_PLAY::GetResourceStruct(UAcom_Vector* Vector, int32 Index) const
{
    if (!Vector || !Vector->IsValidIndex(Index))
    {
        return nullptr;
    }

    return Vector->GetResource(Index);
}

bool APS_PLAY::FUN_Remove_Rs(AActor* Target, int32 RemoveIndex, UAcom_Vector* AcomVector)
{
    if (!Target || !AcomVector)
    {
        return false;
    }

    // 移除资源
    bool bRemoved = AcomVector->RemoveResource(RemoveIndex);
    if (bRemoved)
    {
        // 通知客户端更新UI
        EVE_Client_Update_UMG(Target, AcomVector->GetVectorID(), Target, RemoveIndex);
    }

    return bRemoved;
}

bool APS_PLAY::HandleItemSale(const FResourceStruct* Resource, APlayerController* PC, 
                             UAcom_Vector* Vector, int32 Index)
{
    if (!Resource || !PC || !Vector)
    {
        return false;
    }

    // 获取物品价格
    int32 SalePrice = Resource->Price;
    
    // 增加玩家金钱
    FUN_Server_Money_Add(SalePrice);

    // 触发销售成功事件
    OnItemSold.Broadcast(Resource->ID, SalePrice);

    return true;
}

// ---------- UI更新 ----------
void APS_PLAY::EVE_Client_Update_UMG_Implementation(AActor* Target, int32 VectorID, AActor* Rs, int32 Index)
{
    // 确保在客户端执行
    if (!IsLocalController())
    {
        return;
    }

    // 获取UMG Vector组件
    UUMGVector* UMGVector = Target->FindComponentByClass<UUMGVector>();
    if (!UMGVector || UMGVector->GetVectorID() != VectorID)
    {
        return;
    }

    // 更新UI显示
    UMGVector->UpdateResourceDisplay(Rs, Index);
}

APS_PLAY* APS_PLAY::FUNL_Get_Playerstate_by_PC(APlayerController* PC)
{
    if (!PC)
    {
        return nullptr;
    }

    return Cast<APS_PLAY>(PC->PlayerState);
}

// ---------- 辅助函数 ----------
bool APS_PLAY::ValidateResourceAddition(AActor* Build, int32 ID) const
{
    if (!Build || ID < 0)
    {
        return false;
    }

    UAcom_Vector* Vector = GetBuildAcomVector(Build);
    return Vector != nullptr;
}

UAcom_Vector* APS_PLAY::GetBuildAcomVector(AActor* Build) const
{
    if (!Build)
    {
        return nullptr;
    }

    return Build->FindComponentByClass<UAcom_Vector>();
}

