#include "LearnGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "UObject/ConstructorHelpers.h"

ALearnGameMode::ALearnGameMode()
{
    // ������Ϸ�Ự��
    static ConstructorHelpers::FClassFinder<AGameSession> GameSessionBP(TEXT("/Game/Blueprints/GameSession"));
    if (GameSessionBP.Class != nullptr)
    {
        GameSessionClass = GameSessionBP.Class;
    }

    // ������Ϸ״̬��
    static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBP(TEXT("/Game/Blueprints/GameStateBase"));
    if (GameStateBP.Class != nullptr)
    {
        GameStateClass = GameStateBP.Class;
    }

    // ������ҿ�������
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBP(TEXT("/Game/Blueprints/PC_PLAY"));
    if (PlayerControllerBP.Class != nullptr)
    {
        PlayerControllerClass = PlayerControllerBP.Class;
    }

    // �������״̬��
    static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBP(TEXT("/Game/Blueprints/PS_PLAY"));
    if (PlayerStateBP.Class != nullptr)
    {
        PlayerStateClass = PlayerStateBP.Class;
    }

    // ����HUD��
    static ConstructorHelpers::FClassFinder<AHUD> HudBP(TEXT("/Game/Blueprints/Hud_play"));
    if (HudBP.Class != nullptr)
    {
        HUDClass = HudBP.Class;
    }

    // ����Ĭ��Pawn��
    static ConstructorHelpers::FClassFinder<APawn> PawnBP(TEXT("/Game/Blueprints/BP_player"));
    if (PawnBP.Class != nullptr)
    {
        DefaultPawnClass = PawnBP.Class;
    }

    // ���ù۲���Pawn��
    static ConstructorHelpers::FClassFinder<APawn> SpectatorBP(TEXT("/Game/Blueprints/SpectatorPawn"));
    if (SpectatorBP.Class != nullptr)
    {
        SpectatorClass = SpectatorBP.Class;
    }

    // ����������ҿ�������
    static ConstructorHelpers::FClassFinder<APlayerController> ReplayControllerBP(TEXT("/Game/Blueprints/PlayerController"));
    if (ReplayControllerBP.Class != nullptr)
    {
        ReplaySpectatorPlayerControllerClass = ReplayControllerBP.Class;
    }

    // ���÷�����ͳ�Ƹ�������
    static ConstructorHelpers::FClassFinder<AInfo> ServerStatBP(TEXT("/Game/Blueprints/ServerStatReplicator"));
    if (ServerStatBP.Class != nullptr)
    {
        ServerStatReplicatorClass = ServerStatBP.Class;
    }

    // ������Ϸ��������
    bCanPause = true;           // ������Ϸ��ͣ
    bUseSeamlessTravel = false; // ��ʹ���޷����У���ͼ�л�ʱ��ʾ���ػ���
    bPlayerIsSpectator = false; // ���Ĭ�ϲ��ǹ۲���
    bAllowSpectating = true;    // �������ģʽ
    bDelayedStart = false;      // ��Ϸ�Զ���ʼ������Ҫ�ȴ����׼��
    MinPlayers = 1;             // ������Ҫ1�����
    MaxPlayers = 16;            // �������16�����

    // ��Ϸģʽ��������������
    bStartPlayersAsSpectators = false;  // ��ҽ���ʱ�����Զ���Ϊ����
    bPauseable = true;                  // ��Ϸ������ͣ
}

void ALearnGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // ��Ϸ��ʼʱ�ĳ�ʼ���߼�
    UE_LOG(LogTemp, Log, TEXT("��Ϸģʽ����������ǰ���ã�"));
    UE_LOG(LogTemp, Log, TEXT("����ͣ: %s"), bCanPause ? TEXT("��") : TEXT("��"));
    UE_LOG(LogTemp, Log, TEXT("�޷�����: %s"), bUseSeamlessTravel ? TEXT("����") : TEXT("����"));
    UE_LOG(LogTemp, Log, TEXT("�۲���ģʽ: %s"), bPlayerIsSpectator ? TEXT("����") : TEXT("����"));
}
