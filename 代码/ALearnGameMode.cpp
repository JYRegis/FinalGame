#include "LearnGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "UObject/ConstructorHelpers.h"

ALearnGameMode::ALearnGameMode()
{
    // 设置游戏会话类
    static ConstructorHelpers::FClassFinder<AGameSession> GameSessionBP(TEXT("/Game/Blueprints/GameSession"));
    if (GameSessionBP.Class != nullptr)
    {
        GameSessionClass = GameSessionBP.Class;
    }

    // 设置游戏状态类
    static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBP(TEXT("/Game/Blueprints/GameStateBase"));
    if (GameStateBP.Class != nullptr)
    {
        GameStateClass = GameStateBP.Class;
    }

    // 设置玩家控制器类
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBP(TEXT("/Game/Blueprints/PC_PLAY"));
    if (PlayerControllerBP.Class != nullptr)
    {
        PlayerControllerClass = PlayerControllerBP.Class;
    }

    // 设置玩家状态类
    static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBP(TEXT("/Game/Blueprints/PS_PLAY"));
    if (PlayerStateBP.Class != nullptr)
    {
        PlayerStateClass = PlayerStateBP.Class;
    }

    // 设置HUD类
    static ConstructorHelpers::FClassFinder<AHUD> HudBP(TEXT("/Game/Blueprints/Hud_play"));
    if (HudBP.Class != nullptr)
    {
        HUDClass = HudBP.Class;
    }

    // 设置默认Pawn类
    static ConstructorHelpers::FClassFinder<APawn> PawnBP(TEXT("/Game/Blueprints/BP_player"));
    if (PawnBP.Class != nullptr)
    {
        DefaultPawnClass = PawnBP.Class;
    }

    // 设置观察者Pawn类
    static ConstructorHelpers::FClassFinder<APawn> SpectatorBP(TEXT("/Game/Blueprints/SpectatorPawn"));
    if (SpectatorBP.Class != nullptr)
    {
        SpectatorClass = SpectatorBP.Class;
    }

    // 设置重生玩家控制器类
    static ConstructorHelpers::FClassFinder<APlayerController> ReplayControllerBP(TEXT("/Game/Blueprints/PlayerController"));
    if (ReplayControllerBP.Class != nullptr)
    {
        ReplaySpectatorPlayerControllerClass = ReplayControllerBP.Class;
    }

    // 设置服务器统计复制器类
    static ConstructorHelpers::FClassFinder<AInfo> ServerStatBP(TEXT("/Game/Blueprints/ServerStatReplicator"));
    if (ServerStatBP.Class != nullptr)
    {
        ServerStatReplicatorClass = ServerStatBP.Class;
    }

    // 设置游戏基础属性
    bCanPause = true;           // 允许游戏暂停
    bUseSeamlessTravel = false; // 不使用无缝旅行，地图切换时显示加载画面
    bPlayerIsSpectator = false; // 玩家默认不是观察者
    bAllowSpectating = true;    // 允许观众模式
    bDelayedStart = false;      // 游戏自动开始，不需要等待玩家准备
    MinPlayers = 1;             // 最少需要1名玩家
    MaxPlayers = 16;            // 最多允许16名玩家

    // 游戏模式的其他常用设置
    bStartPlayersAsSpectators = false;  // 玩家进入时不会自动成为观众
    bPauseable = true;                  // 游戏可以暂停
}

void ALearnGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 游戏开始时的初始化逻辑
    UE_LOG(LogTemp, Log, TEXT("游戏模式已启动，当前设置："));
    UE_LOG(LogTemp, Log, TEXT("可暂停: %s"), bCanPause ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, TEXT("无缝旅行: %s"), bUseSeamlessTravel ? TEXT("启用") : TEXT("禁用"));
    UE_LOG(LogTemp, Log, TEXT("观察者模式: %s"), bPlayerIsSpectator ? TEXT("启用") : TEXT("禁用"));
}
