#include "UMG_Player.h"
#include "PS_PLAY.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

UMG_Player::UMG_Player(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // ���캯����ʼ��
}

void UMG_Player::NativeConstruct()
{
    Super::NativeConstruct();
}

FText UMG_Player::GetMoneyText() const
{
    // ��ȡ���״̬
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        // �����ȡʧ�ܣ����ؿ��ı�
        return FText::GetEmpty();
    }

    // ��ȡ��ҽ������
    int32 Money = PlayerState->GetMoney();

    // ��ʽ�������ؽ���ı�
    return FormatMoneyText(Money);
}

FText UMG_Player::FormatMoneyText(int32 Money) const
{
    // ʹ��KismetTextLibrary����ֵת��Ϊ�ı�
    // ������ֵ��ʽ��ѡ�
    // - ��ʹ��ǧλ�ָ���
    // - ��������ʾ����
    // - ������С���������λ��
    return UKismetTextLibrary::Conv_Int64ToText(
        Money,                  // Ҫת������ֵ
        false,                 // bAlwaysSign
        false,                 // bUseGrouping
        MinIntegralDigits,    // MinimumIntegralDigits
        MaxIntegralDigits     // MaximumIntegralDigits
    );
}

APS_PLAY* UMG_Player::GetPlayerState() const
{
    // ��ȡ��ҿ�����
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        return nullptr;
    }

    // �������״̬
    return Cast<APS_PLAY>(PC->PlayerState);
}
