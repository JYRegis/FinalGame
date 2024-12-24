#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LearnGameMode.generated.h"

UCLASS()
class YOURPROJECT_API ALearnGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALearnGameMode();

protected:
    virtual void BeginPlay() override;

    /** 是否允许游戏暂停
     * true: 允许玩家暂停游戏
     * false: 禁止暂停功能
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bCanPause;

    /** 是否使用无缝旅行（地图切换）
     * true: 使用无缝加载，切换地图时不显示加载画面
     * false: 普通加载，切换地图时显示加载画面
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bUseSeamlessTravel;

    /** 玩家是否为观察者模式
     * true: 玩家以观察者身份进入游戏
     * false: 玩家以正常玩家身份进入游戏
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bPlayerIsSpectator;

    /** 是否允许玩家加入为观众
     * true: 允许玩家以观众身份加入
     * false: 禁止观众模式
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bAllowSpectating;

    /** 游戏是否需要玩家开始
     * true: 需要玩家手动开始游戏
     * false: 自动开始游戏
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bDelayedStart;

    /** 游戏的最小玩家数量
     * 设置开始游戏所需的最少玩家数
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    int32 MinPlayers;

    /** 游戏的最大玩家数量
     * 设置游戏允许的最大玩家数
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    int32 MaxPlayers;
};
