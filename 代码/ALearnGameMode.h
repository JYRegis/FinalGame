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

    /** �Ƿ�������Ϸ��ͣ
     * true: ���������ͣ��Ϸ
     * false: ��ֹ��ͣ����
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bCanPause;

    /** �Ƿ�ʹ���޷����У���ͼ�л���
     * true: ʹ���޷���أ��л���ͼʱ����ʾ���ػ���
     * false: ��ͨ���أ��л���ͼʱ��ʾ���ػ���
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bUseSeamlessTravel;

    /** ����Ƿ�Ϊ�۲���ģʽ
     * true: ����Թ۲�����ݽ�����Ϸ
     * false: ��������������ݽ�����Ϸ
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bPlayerIsSpectator;

    /** �Ƿ�������Ҽ���Ϊ����
     * true: ��������Թ�����ݼ���
     * false: ��ֹ����ģʽ
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bAllowSpectating;

    /** ��Ϸ�Ƿ���Ҫ��ҿ�ʼ
     * true: ��Ҫ����ֶ���ʼ��Ϸ
     * false: �Զ���ʼ��Ϸ
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    bool bDelayedStart;

    /** ��Ϸ����С�������
     * ���ÿ�ʼ��Ϸ��������������
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    int32 MinPlayers;

    /** ��Ϸ������������
     * ������Ϸ�������������
     */
    UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
    int32 MaxPlayers;
};
