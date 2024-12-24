#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tasks.generated.h"

UCLASS()
class LEARN_2D_01_API ATasks : public AActor
{
	GENERATED_BODY()

public:
	// ���캯��
	ATasks();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// ��Դ����֪ͨ
	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void NotifyResourceUpdate(int32 ResourceIndex, const FString& ResourceName);

	// �����ͣ��ʾ��Ϣ
	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void DisplayMouseHoverInfo(int32 ElementIndex);

	// ��������߼�
	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void ReceiveTask(AActor* InteractingActor);

private:
	// NPC ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	AActor* NPCActor;

	// �Ƿ����ڽ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	bool bIsInteracting;

	//�Ƿ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	bool bIsTaskComplete;

	// ������ȱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	int32 CurrentProgress;
	// ������ȱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	int32 = RequiredProgress;

	// �洢ÿ��Ԫ�ص�������Ϣ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	TMap<int32, FString> ElementDescriptions; 

	// �洢ÿ��Ԫ�ض�Ӧ�� Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	TMap<int32, AActor*> ElementActors;

	// �洢ÿ��Ԫ�ض�Ӧ�� Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	FOnTaskCompleted OnTaskCompleted;

	// ��ʱ��������������񽻻��߼�
	FTimerHandle TaskTimerHandle;

	// ������ɺ���߼�
	void CompleteTask();

	// ���� NPC ״̬
	void UpdateNPCState();

	// ���������ͣ��Ϣ���߼�
	void UpdateMouseHover(int32 ElementIndex);

	// ����������ɱ�־
	void SetTaskCompleteFlag(bool bIsComplete);

	// �����������
	void UpdateProgress(int32 ProgressAmount);

	// ���� UI ��ʾ����״̬
	void UpdateTaskUI(const FText& StatusMessage);

};
