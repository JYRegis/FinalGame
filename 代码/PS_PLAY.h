#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_PLAY.generated.h"

// ǰ������
class UAcom_Vector;
class UUMGVector;
class UBPINRsNotify;
struct FResourceStruct;

// ί������
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSold, int32, ItemID, int32, Price);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceAdded, AActor*, Build, int32, ID, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewAmount);

UCLASS()
class YOURPROJECT_API APS_PLAY : public APlayerState
{
    GENERATED_BODY()

public:
    APS_PLAY();

    // ---------- ������� ----------
    /** Acom Vector��� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAcom_Vector* Acom_Vector;

    /** UMG Vector��� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UUMGVector* UMG_Vector;

    /** BPIN֪ͨ��� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBPINRsNotify* BPINNotify;

    // ---------- ����ϵͳ ----------
    /** ��ʼ������� */
    UPROPERTY(EditDefaultsOnly, Category = "Currency")
    int32 InitialGold = 1000;

    /** ��ǰ������� */
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Currency")
    int32 Money;

    /** ���������ӽ�Ǯ */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Currency")
    void FUN_Server_Money_Add(int32 Value);

    /** ���������ٽ�Ǯ */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Currency")
    void FUN_Server_Money_Reduce(int32 Value);

    /** ��ȡ��ǰ��Ǯ���� */
    UFUNCTION(BlueprintPure, Category = "Currency")
    int32 GetMoney() const { return Money; }

    /** ����Ƿ����㹻�Ľ�Ǯ */
    UFUNCTION(BlueprintPure, Category = "Currency")
    bool HasEnoughMoney(int32 Amount) const;

    // ---------- �Ϸ�ϵͳ ----------
    /** �������Ϸ��¼� */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
    void EVE_Server_DragDrop(APlayerController* PC, AActor* TargetA, int32 AIndex, 
                            AActor* TargetB, int32 BIndex);

    /** ִ���ϷŲ��� */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void FUN_Server_Drag_Drop(APS_PLAY* Target, AActor* TargetA, int32 AIndex, 
                             AActor* TargetB, int32 BIndex);

    // ---------- ��Դ���� ----------
    /** �����������Դ��Build */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Resources")
    void EVE_Add_Rs_ToBuild(AActor* Build, int32 ID);

    /** �����Դ���� */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool FUN_Add_Rs(AActor* Target, int32 ID, int32& OutIndex);

    /** ������������Ʒ */
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Trading")
    void EVE_Sell_Rs(int32 Index, APlayerController* PC, UAcom_Vector* Vector);

    /** ��ȡ��Դ�ṹ�� */
    UFUNCTION(BlueprintPure, Category = "Resources")
    FResourceStruct* GetResourceStruct(UAcom_Vector* Vector, int32 Index) const;

    /** �Ƴ���Դ */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool FUN_Remove_Rs(AActor* Target, int32 RemoveIndex, UAcom_Vector* AcomVector);

    // ---------- UI���� ----------
    /** �ͻ��˸���UMG */
    UFUNCTION(Client, Reliable)
    void EVE_Client_Update_UMG(AActor* Target, int32 VectorID, AActor* Rs, int32 Index);

    /** ��ȡ���״̬ */
    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    static APS_PLAY* FUNL_Get_Playerstate_by_PC(APlayerController* PC);

    // ---------- �¼�ί�� ----------
    /** ��Ʒ�۳��¼� */
    UPROPERTY(BlueprintAssignable, Category = "Trading")
    FOnItemSold OnItemSold;

    /** ��Դ����¼� */
    UPROPERTY(BlueprintAssignable, Category = "Resources")
    FOnResourceAdded OnResourceAdded;

    /** ��Ǯ�仯�¼� */
    UPROPERTY(BlueprintAssignable, Category = "Currency")
    FOnMoneyChanged OnMoneyChanged;

protected:
    // ---------- ���縴�� ----------
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** ��Ǯ�����仯�ص� */
    UFUNCTION()
    virtual void OnRep_Money();

    // ---------- �ڲ��������� ----------
    /** ���ý�Ǯ���� */
    void SetMoney(int32 NewValue);

    /** ��֤�ϷŲ��� */
    bool ValidateDragDrop(AActor* TargetA, int32 AIndex, AActor* TargetB, int32 BIndex) const;

    /** ������Ʒ���� */
    bool HandleItemSwap(UAcom_Vector* VectorA, int32 AIndex, UAcom_Vector* VectorB, int32 BIndex);

    /** ������Ʒ���� */
    bool HandleItemPurchase(APlayerController* PC, UAcom_Vector* ShopVector, int32 ShopIndex, 
                           UAcom_Vector* InventoryVector, int32 InventoryIndex);

    /** ������Ʒ���� */
    bool HandleItemSale(const FResourceStruct* Resource, APlayerController* PC, 
                       UAcom_Vector* Vector, int32 Index);

    /** ��֤��Դ��� */
    bool ValidateResourceAddition(AActor* Build, int32 ID) const;

    /** ��ȡBuild��Acom Vector��� */
    UAcom_Vector* GetBuildAcomVector(AActor* Build) const;
};
