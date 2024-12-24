#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Bp.generated.h"

class UUMG_Vector;
class APS_PLAY;
class APC_PLAY;

/**
 * ��������ö��
 * ���ڱ�ʶ��ͬ���͵Ľ�����
 */
UENUM(BlueprintType)
enum class EBuildType : uint8
{
    None = 0,
    Exchange UMETA(DisplayName = "����վ"),
    // �ڴ����������������
};

/**
 * UMG_Bp��
 * �������ͽ����˵���UI����
 */
UCLASS()
class YOURPROJECT_API UUMG_Bp : public UUserWidget
{
    GENERATED_BODY()

public:
    /** ���캯�� */
    UMG_Bp(const FObjectInitializer& ObjectInitializer);

    /** ���½����˵� */
    UFUNCTION(BlueprintCallable, Category = "Building")
    void FUN_Update_Open();

protected:
    /** ���湹��ʱ���� */
    virtual void NativeConstruct() override;

    // -------- UI������� --------
    /** ����Vector�ؼ����� */
    UPROPERTY(meta = (BindWidget))
    UUMG_Vector* Umg_Vector_Bp;

    /** ���Ž���˵�Vector�ؼ� */
    UPROPERTY(meta = (BindWidget))
    UUMG_Vector* Umg_Vector_Open;

    // -------- ��ʱ����� --------
    /** �ӳٸ��¶�ʱ����� */
    FTimerHandle UpdateTimerHandle;

    /** �ӳ�ʱ�� */
    const float DelayDuration = 0.2f;

    // -------- �ӳٸ��¹��� --------
    /** �ӳٸ��º��� */
    UFUNCTION()
    void DelayedUpdate();

    /** ��֤�����±��� */
    UFUNCTION()
    void ValidateAndUpdate();

    // -------- �������¹��� --------
    /** ���±������� */
    UFUNCTION()
    void UpdateBackpackGrid();

    /** ���±�����Դ */
    UFUNCTION()
    void UpdateBackpackResources();

    // -------- �����˵����� --------
    /** ��ȡ�������� */
    UFUNCTION()
    EBuildType GetBuildType() const;

    /** ���ý������� */
    UFUNCTION()
    void SetBuildIndex(int32 Index);

    /** ���½������� */
    UFUNCTION()
    void UpdateBuildGrid(EBuildType BuildType);

    /** ���½�����Դ */
    UFUNCTION()
    void UpdateBuildResources();

    // -------- �������� --------
    /** ��ȡ���״̬ */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

    /** ��ȡ����ԱActor */
    UFUNCTION()
    AActor* GetOperatorActor() const;
};
