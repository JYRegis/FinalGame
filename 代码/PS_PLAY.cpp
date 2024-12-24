#include "PS_PLAY.h"
#include "Acom_Vector.h"
#include "UMGVector.h"
#include "BPINRsNotify.h"
#include "ResourceStruct.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"

APS_PLAY::APS_PLAY()
{
    // ������Ҫ�����
    Acom_Vector = CreateDefaultSubobject<UAcom_Vector>(TEXT("Acom_Vector"));
    UMG_Vector = CreateDefaultSubobject<UUMGVector>(TEXT("UMG_Vector"));
    BPINNotify = CreateDefaultSubobject<UBPINRsNotify>(TEXT("BPINNotify"));

    // ���ó�ʼ��Ǯ
    Money = InitialGold;
}

// ---------- ���縴�� ----------
void APS_PLAY::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ע��Money����Ϊ�ɸ��ƣ���ʹ��OnRep_Money��Ϊ�ص�
    DOREPLIFETIME_CONDITION_NOTIFY(APS_PLAY, Money, COND_None, REPNOTIFY_OnChanged);
}

// ---------- ����ϵͳ ----------
void APS_PLAY::FUN_Server_Money_Add_Implementation(int32 Value)
{
    // ȷ���ڷ�������ִ��
    if (!HasAuthority())
    {
        return;
    }

    // ��ȡ��ǰ��Ǯ����������
    int32 CurrentMoney = GetMoney();
    int32 NewMoney = CurrentMoney + Value;
    
    // �����µĽ�Ǯ����
    SetMoney(NewMoney);
}

void APS_PLAY::FUN_Server_Money_Reduce_Implementation(int32 Value)
{
    // ȷ���ڷ�������ִ��
    if (!HasAuthority())
    {
        return;
    }

    // ����Ƿ����㹻�Ľ�Ǯ
    if (HasEnoughMoney(Value))
    {
        // �����µĽ�Ǯ����
        int32 CurrentMoney = GetMoney();
        int32 NewMoney = CurrentMoney - Value;
        
        // �����µĽ�Ǯ����
        SetMoney(NewMoney);
        
        // ���سɹ�
        OnMoneyChanged.Broadcast(NewMoney);
    }
}

void APS_PLAY::SetMoney(int32 NewValue)
{
    // ȷ����Ǯ����Ϊ����
    NewValue = FMath::Max(0, NewValue);
    
    // �����Ǯ���������仯
    if (Money != NewValue)
    {
        Money = NewValue;
        
        // ����ڷ������ϣ�����仯���Զ����Ƶ��ͻ���
        if (HasAuthority())
        {
            OnMoneyChanged.Broadcast(Money);
        }
        
        // ������Ǯ�仯�Ļص�
        OnRep_Money();
    }
}

void APS_PLAY::OnRep_Money()
{
    // �����ﴦ���Ǯ�仯���UI���µ��߼�
    // ���紥��ί�л��¼�������UI
    OnMoneyChanged.Broadcast(Money);
}

bool APS_PLAY::HasEnoughMoney(int32 Amount) const
{
    return Money >= Amount;
}

// ---------- �Ϸ�ϵͳ ----------
void APS_PLAY::EVE_Server_DragDrop_Implementation(APlayerController* PC, AActor* TargetA, 
    int32 AIndex, AActor* TargetB, int32 BIndex)
{
    // ȷ���ڷ�������ִ��
    if (!HasAuthority())
    {
        return;
    }

    // ��֤����
    if (!ValidateDragDrop(TargetA, AIndex, TargetB, BIndex))
    {
        return;
    }

    // ִ���ϷŲ���
    FUN_Server_Drag_Drop(this, TargetA, AIndex, TargetB, BIndex);
}

void APS_PLAY::FUN_Server_Drag_Drop(APS_PLAY* Target, AActor* TargetA, int32 AIndex, 
    AActor* TargetB, int32 BIndex)
{
    // ��ȡ����Ŀ���Acom Vector���
    UAcom_Vector* VectorA = TargetA->FindComponentByClass<UAcom_Vector>();
    UAcom_Vector* VectorB = TargetB->FindComponentByClass<UAcom_Vector>();

    if (!VectorA || !VectorB)
    {
        return;
    }

    // ����Ƿ����̵꽻��
    bool bIsShopTransaction = VectorA->IsShop() || VectorB->IsShop();
    
    if (bIsShopTransaction)
    {
        // �������߼�
        UAcom_Vector* ShopVector = VectorA->IsShop() ? VectorA : VectorB;
        UAcom_Vector* InventoryVector = VectorA->IsShop() ? VectorB : VectorA;
        int32 ShopIndex = VectorA->IsShop() ? AIndex : BIndex;
        int32 InventoryIndex = VectorA->IsShop() ? BIndex : AIndex;

        if (HandleItemPurchase(Target->GetOwningController(), ShopVector, ShopIndex, 
                             InventoryVector, InventoryIndex))
        {
            // ����ɹ�������UI
            EVE_Client_Update_UMG(TargetA, VectorA->GetVectorID(), TargetA, AIndex);
            EVE_Client_Update_UMG(TargetB, VectorB->GetVectorID(), TargetB, BIndex);
        }
    }
    else
    {
        // ������ͨ��Ʒ����
        if (HandleItemSwap(VectorA, AIndex, VectorB, BIndex))
        {
            // �����ɹ�������UI
            EVE_Client_Update_UMG(TargetA, VectorA->GetVectorID(), TargetA, AIndex);
            EVE_Client_Update_UMG(TargetB, VectorB->GetVectorID(), TargetB, BIndex);
        }
    }
}

bool APS_PLAY::ValidateDragDrop(AActor* TargetA, int32 AIndex, AActor* TargetB, int32 BIndex) const
{
    // ������֤
    if (!TargetA || !TargetB || AIndex < 0 || BIndex < 0)
    {
        return false;
    }

    // ������
    UAcom_Vector* VectorA = TargetA->FindComponentByClass<UAcom_Vector>();
    UAcom_Vector* VectorB = TargetB->FindComponentByClass<UAcom_Vector>();

    if (!VectorA || !VectorB)
    {
        return false;
    }

    // ��������Ƿ���Ч
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

    // ��ȡ��Ʒ����
    FResourceStruct* ItemA = GetResourceStruct(VectorA, AIndex);
    FResourceStruct* ItemB = GetResourceStruct(VectorB, BIndex);

    if (!ItemA || !ItemB)
    {
        return false;
    }

    // ִ�н���
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

    // ��ȡ��Ʒ��Ϣ
    FResourceStruct* ShopItem = GetResourceStruct(ShopVector, ShopIndex);
    if (!ShopItem)
    {
        return false;
    }

    // �������Ƿ����㹻�Ľ�Ǯ
    if (!HasEnoughMoney(ShopItem->Price))
    {
        // ���������������ʾ
        return false;
    }

    // �۳���Ǯ
    FUN_Server_Money_Reduce(ShopItem->Price);

    // ת����Ʒ
    return InventoryVector->AddResource(ShopItem->ID, InventoryIndex);
}
// ---------- ��Դ���� ----------
void APS_PLAY::EVE_Add_Rs_ToBuild_Implementation(AActor* Build, int32 ID)
{
    // ȷ���ڷ�������ִ��
    if (!HasAuthority())
    {
        return;
    }

    // ��֤����
    if (!ValidateResourceAddition(Build, ID))
    {
        return;
    }

    // ��ȡAcom Vector���
    UAcom_Vector* Vector = GetBuildAcomVector(Build);
    if (!Vector)
    {
        return;
    }

    // �����Դ
    int32 Index;
    if (FUN_Add_Rs(Build, ID, Index))
    {
        // ������Դ����¼�
        OnResourceAdded.Broadcast(Build, ID, Index);
    }
}

bool APS_PLAY::FUN_Add_Rs(AActor* Target, int32 ID, int32& OutIndex)
{
    if (!Target)
    {
        return false;
    }

    // ��ȡAcom Vector���
    UAcom_Vector* Vector = GetBuildAcomVector(Target);
    if (!Vector)
    {
        return false;
    }

    // ��ȡ��һ����������
    OutIndex = Vector->GetNextAvailableIndex();
    if (OutIndex == -1)
    {
        // û�п���λ��
        return false;
    }

    // �����Դ
    bool bSuccess = Vector->AddResource(ID, OutIndex);
    if (bSuccess)
    {
        // ���¿ͻ���UI
        EVE_Client_Update_UMG(Target, Vector->GetVectorID(), Target, OutIndex);
    }

    return bSuccess;
}

void APS_PLAY::EVE_Sell_Rs_Implementation(int32 Index, APlayerController* PC, UAcom_Vector* Vector)
{
    // ȷ���ڷ�������ִ��
    if (!HasAuthority())
    {
        return;
    }

    // ��ȡ��Դ�ṹ��
    FResourceStruct* Resource = GetResourceStruct(Vector, Index);
    if (!Resource)
    {
        return;
    }

    // ������Ʒ����
    if (HandleItemSale(Resource, PC, Vector, Index))
    {
        // ���۳ɹ����Ƴ���Ʒ
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

    // �Ƴ���Դ
    bool bRemoved = AcomVector->RemoveResource(RemoveIndex);
    if (bRemoved)
    {
        // ֪ͨ�ͻ��˸���UI
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

    // ��ȡ��Ʒ�۸�
    int32 SalePrice = Resource->Price;
    
    // ������ҽ�Ǯ
    FUN_Server_Money_Add(SalePrice);

    // �������۳ɹ��¼�
    OnItemSold.Broadcast(Resource->ID, SalePrice);

    return true;
}

// ---------- UI���� ----------
void APS_PLAY::EVE_Client_Update_UMG_Implementation(AActor* Target, int32 VectorID, AActor* Rs, int32 Index)
{
    // ȷ���ڿͻ���ִ��
    if (!IsLocalController())
    {
        return;
    }

    // ��ȡUMG Vector���
    UUMGVector* UMGVector = Target->FindComponentByClass<UUMGVector>();
    if (!UMGVector || UMGVector->GetVectorID() != VectorID)
    {
        return;
    }

    // ����UI��ʾ
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

// ---------- �������� ----------
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

