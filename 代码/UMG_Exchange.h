#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "UMG_Exchange.generated.h"

class APS_PLAY;
class APC_PLAY;
class UUMG_Rs;

/**
 * UMG_Exchange��
 * ����վ����ؼ�
 * ������Դ���ס��ϷŲ����ͽ������
 */
UCLASS()
class YOURPROJECT_API UUMG_Exchange : public UUserWidget
{
    GENERATED_BODY()

public:
    /** ���캯�� */
    UMG_Exchange(const FObjectInitializer& ObjectInitializer);

    /** ���湹��ʱ���� */
    virtual void NativeConstruct() override;

    /** �����Ϸ��¼� */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation);

    /** ���½���վ���� */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void FUN_Update_Exchange();

protected:
    // -------- UI��� --------
    /** ����վ��Դ�б� */
    UPROPERTY()
    TArray<UUMG_Rs*> ExchangeItems;

    /** ��װ������ - ���ڴ洢��Դ��Ŀ */
    UPROPERTY(meta = (BindWidget))
    class UWrapBox* WrapBox_42;

    // -------- �ϷŹ��� --------
    /** ��֤�ϷŲ��� */
    UFUNCTION()
    bool ValidateDropOperation(UDragDropOperation* Operation, UUMG_Rs*& OutResource) const;

    /** ������Դ���� */
    UFUNCTION()
    bool HandleResourceSell(UUMG_Rs* DroppedResource, int32 ActorID, int32 Index);

    // -------- ���¹��� --------
    /** ����������Դ��Ŀ */
    UFUNCTION()
    void ClearExistingItems();

    /** �����µ���Դ��Ŀ */
    UFUNCTION()
    void CreateResourceItems(const TArray<FResourceStruct>& Resources, int32 ActorID);

    /** ������Դ��Ϣ */
    UFUNCTION()
    void SetResourceInfo(UUMG_Rs* ResourceWidget, const FResourceStruct& ResourceData, int32 ActorID, int32 Index);

    // -------- �������� --------
    /** ��ȡ���״̬ */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

    /** ��ȡ��ҿ����� */
    UFUNCTION()
    APC_PLAY* GetPlayerController() const;

private:
    /** ��Դ��Ŀ�� */
    UPROPERTY()
    TSubclassOf<UUMG_Rs> ResourceItemClass;
};
