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

    // ---------- �ϷŹ��� ----------
    /** �����Ϸ��¼� */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    bool OnDrop(FGeometry MyGeometry, FPointerEvent PointerEvent, UDragDropOperation* Operation);

    /** �����϶�����¼� */
    UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
    void OnDragDetected(FGeometry MyGeometry, const FPointerEvent& PointerEvent);

    /** ������갴���¼� */
    UFUNCTION(BlueprintCallable, Category = "Mouse Events")
    FReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

    // ---------- ��Դ���� ----------
    /** ������Դ��Ϣ */
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void FUN_SetRsInfo(const FResourceStruct& ResourceStruct);

    /** ��ȡ��Դ���� */
    UFUNCTION(BlueprintCallable, Category = "Resource")
    const FResourceStruct* GetResourceData() const { return CurrentResource; }

    // ---------- �������� ----------
    /** ����Actor ID */
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void SetActorID(int32 NewID) { ActorID = NewID; }

    /** �������� */
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void SetIndex(int32 NewIndex) { Index = NewIndex; }

    /** ������Դͼ�� */
    UFUNCTION(BlueprintCallable, Category = "Resource")
    void SetResourceImage(UTexture2D* NewImage);

protected:
    // ---------- UI��� ----------
    /** ��Դͼ�� */
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Icon;

    /** ��ԴͼƬ */
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_169;

    /** ��Դ�ı� */
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Resource;

    /** �۸��ı� */
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Text_Price;

    // ---------- ���ݳ�Ա ----------
    /** Actor ID */
    UPROPERTY(BlueprintReadOnly, Category = "Resource")
    int32 ActorID;

    /** ��Դ���� */
    UPROPERTY(BlueprintReadOnly, Category = "Resource")
    int32 Index;

    /** ��ǰ��Դ���� */
    UPROPERTY()
    FResourceStruct* CurrentResource;

    // ---------- �������� ----------
    /** ��ȡ���״̬ */
    APS_PLAY* GetPlayerState() const;

    /** ��ȡ��ҿ����� */
    APC_PLAY* GetPlayerController() const;

    /** �����ϷŲ��� */
    UDragDropOperation* CreateDragDropOperation();

    /** �����Ϸ��Ӿ�Ч�� */
    UUMG_RS* CreateDragVisual();

    /** �����Դ�Ƿ���Ч */
    bool IsValid() const;

    /** ����϶����� */
    FReply DetectDragOperation(const FPointerEvent& MouseEvent);

    /** ������Դͼ�� */
    void UpdateResourceIcon(UTexture2D* IconTexture);

    /** ���ÿؼ��ɼ��� */
    void SetResourceVisibility(ESlateVisibility NewVisibility);

    /** ���ÿؼ��Ƿ���� */
    void SetResourceEnabled(bool bEnabled);

    /** ���ÿؼ��ɼ��� */
    void SetWidgetVisibility(UWidget* Widget, ESlateVisibility NewVisibility);
};
