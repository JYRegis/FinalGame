#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Player.generated.h"

class APS_PLAY;

/**
 * UMG_Player类
 * 用于显示玩家信息（如金币数量）的UI控件
 */
UCLASS()
class YOURPROJECT_API UUMG_Player : public UUserWidget
{
    GENERATED_BODY()

public:
    UMG_Player(const FObjectInitializer& ObjectInitializer);

    /** 获取金币显示文本 */
    UFUNCTION(BlueprintCallable, Category = "Player UI")
    FText GetMoneyText() const;

protected:
    /** 界面构造时调用 */
    virtual void NativeConstruct() override;

    /** 获取玩家状态 */
    UFUNCTION()
    APS_PLAY* GetPlayerState() const;

    /** 将金币数值转换为显示文本 */
    UFUNCTION()
    FText FormatMoneyText(int32 Money) const;

private:
    /** 金币数值显示的最小整数位数 */
    const int32 MinIntegralDigits = 1;

    /** 金币数值显示的最大整数位数 */
    const int32 MaxIntegralDigits = 324;
};
