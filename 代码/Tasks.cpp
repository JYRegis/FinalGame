#include "Tasks.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "TaskStatusComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


// ���캯��
ATasks::ATasks()
{
	// ���� Tick
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ������
	NPCActor = nullptr;
	bIsInteracting = false;
	bIsTaskComplete = false;
	CurrentProgress = 0;
	RequiredProgress = 100;
	HoverTextBlock = nullptr;
	MouseHoverWidget = nullptr;
}

// ��Ϸ��ʼʱ����
void ATasks::BeginPlay()
{
	Super::BeginPlay();
	// ��ʼ�� UI С����
	if (MouseHoverWidgetClass)
	{
		MouseHoverWidget = CreateWidget<UUserWidget>(GetWorld(), MouseHoverWidgetClass);
		if (MouseHoverWidget)
		{
			MouseHoverWidget->AddToViewport();
			HoverTextBlock = Cast<UTextBlock>(MouseHoverWidget->GetWidgetFromName(TEXT("HoverTextBlock")));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("��ʼ"));
}

// ÿ֡����
void ATasks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ����������ڽ��У����Զ�̬�����������
	if (bIsInteracting){
		UpdateProgress(DeltaTime * 10);
	}
}

// ��Դ����֪ͨ
void ATasks::NotifyResourceUpdate(int32 ResourceIndex, const FString& ResourceName)
{
	// ����������ȣ�ĳЩ��Դ��������أ�
	if (ResourceName == "TaskResource")
	{
		UpdateProgress(10); 
	}
	if (NPCActor)
	{
		UpdateNPCState();
	}
}

// �����ͣ��ʾ��Ϣ
void ATasks::DisplayMouseHoverInfo(int32 ElementIndex)
{
	UpdateMouseHover(ElementIndex);
}

// ���������ͣ��Ϣ
void ATasks::UpdateMouseHover(int32 ElementIndex)
{
	if (ElementIndex >= 0)
	{
		FString HoverInfo = FString::Printf(TEXT("Ԫ�� %d: ����ͣ"), ElementIndex);

		// ���� UI �ϵ�����
		if (HoverTextBlock)
		{
			HoverTextBlock->SetText(FText::FromString(HoverInfo));
		}

		UE_LOG(LogTemp, Log, TEXT("�����ͣ��Ϣ�Ѹ��£�Ԫ������: %d"), ElementIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("��Ч"));
	}
}

// ��������߼�
void ATasks::ReceiveTask(AActor* InteractingActor)
{
	if (!InteractingActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("����"));
		return;
	}

	NPCActor = InteractingActor;
	bIsInteracting = true;

	// ģ��������̵���ʱ����
	GetWorldTimerManager().SetTimer(TaskTimerHandle, this, &ATasks::CompleteTask, 2.0f, false);
}

// ������ɺ���߼�
void ATasks::CompleteTask()
{
	if (bIsInteracting)
	{
		SetTaskCompleteFlag(true);
		UpdateNPCState();
		bIsInteracting = false;

		UE_LOG(LogTemp, Log, TEXT("���"));
	}
}

// ���� NPC ״̬
void ATasks::UpdateNPCState()
{
	if (NPCActor)
	{
		UTaskStatusComponent* TaskStatus = NPCActor->FindComponentByClass<UTaskStatusComponent>();
		if (TaskStatus)
		{
			TaskStatus->SetTaskStatus(ETaskStatus::InProgress);
			UE_LOG(LogTemp, Log, TEXT("NPC ״̬�Ѹ���: %s"), *NPCActor->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("����"));
	}
}

// ����������ɱ�־
void ATasks::SetTaskCompleteFlag(bool bIsComplete)
{
	bIsTaskComplete = bIsComplete;

	if (bIsComplete)
	{
		UE_LOG(LogTemp, Log, TEXT("���"));
		// ���� UI ��ʾ����״̬
		UpdateTaskUI(FText::FromString(TEXT("��������ɣ�")));
		// ������������¼�
		OnTaskCompleted.Broadcast();
		// ���� NPC ״̬
		if (NPCActor)
		{
			UTaskStatusComponent* TaskStatus = NPCActor->FindComponentByClass<UTaskStatusComponent>();
			if (TaskStatus)
			{
				TaskStatus->SetTaskStatus(ETaskStatus::Completed);
				UE_LOG(LogTemp, Log, TEXT("NPC ״̬�Ѹ���Ϊ���"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("δ���"));
	}
}

// ���� UI ��ʾ����״̬
void ATasks::UpdateTaskUI(const FText& StatusMessage)
{
	if (TaskStatusTextBlock)
	{
		TaskStatusTextBlock->SetText(StatusMessage);
		UE_LOG(LogTemp, Log, TEXT("����״̬ UI �Ѹ���: %s"), *StatusMessage.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("����״̬ UI δ��"));
	}
}
// �����������
void ATasks::UpdateProgress(int32 ProgressAmount)
{
	if (bIsInteracting && !bIsTaskComplete)
	{
		CurrentProgress += ProgressAmount;

		if (CurrentProgress >= RequiredProgress)
		{
			CurrentProgress = RequiredProgress;
			CompleteTask();
		}

		UE_LOG(LogTemp, Log, TEXT("��ǰ����: %d%%"), CurrentProgress);
	}
}