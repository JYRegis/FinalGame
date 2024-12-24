#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Player.generated.h"

class APS_PLAY;

/**
 * UMG_Player��
 * ������ʾ�����Ϣ��������������UI�ؼ�
 */
UCLASS()
class YOURPROJECT_API UUMG_Player : public UUserWidget
{
    GENERATED_BODY()

public:
    UMG_Player(const FObjectInitializer& ObjectInitializer);

    /** ��ȡ�����ʾ�ı� */
    UFUNCTION(BlueprintCallable, Category = "Player UI")
    FText GetMoneyText() const;

protected:
    /** ���湹��ʱ���� */
    virtual void NativeConstruct() override;

    /** ��ȡ���״̬ */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

    /** �������ֵת��Ϊ��ʾ�ı� */
    UFUNCTION()
    FText FormatMoneyText(int32 Money) const;

private:
    /** �����ֵ��ʾ����С����λ�� */
    const int32 MinIntegralDigits = 1;

    /** �����ֵ��ʾ���������λ�� */
    const int32 MaxIntegralDigits = 324;
};
