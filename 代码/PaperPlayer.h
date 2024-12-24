#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PaperPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class LEARN_2D_01_API APaperPlayer : public APaperZDCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	int32 CurrentTool; // ��ǰ���߱��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	float XDirection; // X����ĸ���ֵ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	float YDirection; // Y����ĸ���ֵ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	int32 Level; // �ȼ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsFishingTool1; // �ɽ�����ͷ1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsFishingTool2; // �ɽ�����ͷ2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsFishing; // �Ƿ���Խ��������㣩

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> AmmoWidgetClass; // ��ҩ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> FishingWidgetClass; // ����UI��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> BackpackWidgetClass; // ����UI��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsCooking; // �Ƿ�������״̬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> TaskListWidgetClass; // �����б�ؼ���
public:

	// ��ȡ���2D����
	UFUNCTION(BlueprintPure)
	FVector2D GetMouse2dVector();

	// ��ȡ���2D��������һ��
	UFUNCTION(BlueprintPure)
	FVector2D Mouse2dVectorNormalize();

	// ��ȡ����2D��������һ��
	UFUNCTION(BlueprintPure)
	FVector2D Player2dVectorNormalize();

	// ������
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// �ƶ��߼�
	void Move(const FInputActionValue& Value);

	// �����л��߼�
	void Mouseicon();
	void Mousehoe();

	// �������ܷ�������
	void SwitchTool1(); // �л�����1
	void SwitchTool2(); // �л�����2
	void SwitchTool3(); // �л�����3
	void OpenBackpack(); // �򿪱���
	void ToggleWoodTool(); // �л���ľ����
	void ToggleMiningTool(); // �л��ɿ󹤾�
	void PauseWoodAction(); // ��ͣ��ľ��Ϊ
	void PauseOreAction(); // ��ͣ�ɿ���Ϊ

	// ���붯��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Experimentaltool;

	// ��ȡ��ͷ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BlueprintVar1;

	// ��ȡ��ͷ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BlueprintVar2;

	// ������Ա����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BlueprintVar3; // ����3

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> BackpackWidgetClass; // ����������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* BackpackWidget; // ����ʵ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool bWoodTool; // ��ľ����״̬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool bMiningTool; // �ɿ󹤾�״̬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> TimerWidgetClass; // ʱ�䲿����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* TimerWidget; // ʱ�䲿��ʵ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> AmmoWidgetClass; // ��ҩ��ʾUI��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* AmmoWidget; // ��ҩ��ʾUIʵ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> RuneWidgetClass; // ������ʾUI��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* RuneWidget; // ������ʾUIʵ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> BackpackWidgetClass; // ������ʾUI��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* BackpackWidget; // ������ʾUIʵ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<AActor> CurrentTool; // ��ǰ����
};

