#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Vector.generated.h"

class UUMG_Rs;
class APC_PLAY;
class FResourceStruct;
class UAcom_Vector;

UCLASS()
class YOURPROJECT_API UUMG_Vector : public UUserWidget
{
    GENERATED_BODY()

public:
    UUMG_Vector(const FObjectInitializer& ObjectInitializer);
    
    virtual void NativeConstruct() override;

    // ---------- ������¹��� ----------
    /** ���������� */
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void FUN_Update_Grid(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID);

    /** �����ض���Դ����Ϣ */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void FUN_Update_Rs(const FResourceStruct& ResourceStruct, int32 Index);

    /** ��������������Դ */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void FUN_Update_Rss(int32 ActorID);

protected:
    // ---------- UI��� ----------
    /** WrapBox�������� */
    UPROPERTY(meta = (BindWidget))
    class UWrapBox* WrapBox_42;

    /** �洢UMG��Դʵ�������� */
    UPROPERTY()
    TArray<UUMG_Rs*> UMG_Rss;

    // ---------- �������� ----------
    /** ����������� */
    void ClearGrid();

    /** ��ȡ��ҿ����� */
    APC_PLAY* GetPlayerController() const;

    /** Ϊÿ����Դ���������UMG */
    void CreateAndAddUMGForResources(const TArray<UUMG_Rs*>& UMGRsArray, int32 ActorID);

    /** ����������ȡUMG��Դʵ�� */
    UUMG_Rs* GetUMGRsAtIndex(int32 Index) const;

    /** ������Դ��Ϣ */
    void FUN_Set_Rs_Info(UUMG_Rs* TargetRs, const FResourceStruct& ResourceStruct);

    /** ��ȡVector����е���Դ���� */
    TArray<FResourceStruct> GetResourcesFromActor(AActor* Actor) const;

    /** ��ȡActor��Vector��� */
    UAcom_Vector* GetVectorAcom(AActor* Actor) const;
};
