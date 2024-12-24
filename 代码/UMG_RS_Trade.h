#pragma once

#include "CoreMinimal.h"
#include "UMG_RS.h"
#include "Blueprint/DragDropOperation.h"
#include "UMG_RS_Trade.generated.h"

class APS_PLAY;

/**
 * UMG_RS_Trade��
 * ���׽����е���Դ��Ŀ�ؼ�
 * ������Դ����ʾ���Ϸź͹������
 */
UCLASS()
class YOURPROJECT_API UUMG_RS_Trade : public UUMG_RS
{
    GENERATED_BODY()

public:
    /** ���캯�� */
    UMG_RS_Trade(const FObjectInitializer& ObjectInitializer);

    /** �����Ϸ��¼� */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    virtual bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation) override;

protected:
    /** ���湹��ʱ���� */
    virtual void NativeConstruct() override;

    // -------- UI��� --------
    /** ����ť���� */
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_0;

    // -------- ��ť�¼� --------
    /** ����ť����¼� */
    UFUNCTION()
    void OnBuyButtonClicked();

    /** �󶨰�ť�¼� */
    UFUNCTION()
    void BindButtonEvents();

    // -------- ���׹��� --------
    /** ִ�й������ */
    UFUNCTION()
    void ExecuteBuyOperation();

    /** ��֤�ϷŲ��� */
    UFUNCTION()
    bool ValidateDropOperation(UDragDropOperation* Operation) const;

    // -------- �������� --------
    /** ��ȡ���״̬ */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

private:
    /** ��Դ�ϷŲ����Ľ������ */
    bool HandleResourceDrop(UUMG_RS* DroppedResource);
};
