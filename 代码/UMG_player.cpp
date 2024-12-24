#include "UMG_Player.h"
#include "PS_PLAY.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

UMG_Player::UMG_Player(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 构造函数初始化
}

void UMG_Player::NativeConstruct()
{
    Super::NativeConstruct();
}

FText UMG_Player::GetMoneyText() const
{
    // 获取玩家状态
    APS_PLAY* PlayerState = GetPlayerState();
    if (!PlayerState)
    {
        // 如果获取失败，返回空文本
        return FText::GetEmpty();
    }

    // 获取玩家金币数量
    int32 Money = PlayerState->GetMoney();

    // 格式化并返回金币文本
    return FormatMoneyText(Money);
}

FText UMG_Player::FormatMoneyText(int32 Money) const
{
    // 使用KismetTextLibrary将数值转换为文本
    // 设置数值格式化选项：
    // - 不使用千位分隔符
    // - 不总是显示正号
    // - 设置最小和最大整数位数
    return UKismetTextLibrary::Conv_Int64ToText(
        Money,                  // 要转换的数值
        false,                 // bAlwaysSign
        false,                 // bUseGrouping
        MinIntegralDigits,    // MinimumIntegralDigits
        MaxIntegralDigits     // MaximumIntegralDigits
    );
}

APS_PLAY* UMG_Player::GetPlayerState() const
{
    // 获取玩家控制器
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        return nullptr;
    }

    // 返回玩家状态
    return Cast<APS_PLAY>(PC->PlayerState);
}
