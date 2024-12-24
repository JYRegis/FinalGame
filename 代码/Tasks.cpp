#include "Tasks.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "TaskStatusComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


// 构造函数
ATasks::ATasks()
{
	// 启用 Tick
	PrimaryActorTick.bCanEverTick = true;

	// 初始化变量
	NPCActor = nullptr;
	bIsInteracting = false;
	bIsTaskComplete = false;
	CurrentProgress = 0;
	RequiredProgress = 100;
	HoverTextBlock = nullptr;
	MouseHoverWidget = nullptr;
}

// 游戏开始时调用
void ATasks::BeginPlay()
{
	Super::BeginPlay();
	// 初始化 UI 小部件
	if (MouseHoverWidgetClass)
	{
		MouseHoverWidget = CreateWidget<UUserWidget>(GetWorld(), MouseHoverWidgetClass);
		if (MouseHoverWidget)
		{
			MouseHoverWidget->AddToViewport();
			HoverTextBlock = Cast<UTextBlock>(MouseHoverWidget->GetWidgetFromName(TEXT("HoverTextBlock")));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("开始"));
}

// 每帧调用
void ATasks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 如果任务正在进行，可以动态更新任务进度
	if (bIsInteracting){
		UpdateProgress(DeltaTime * 10);
	}
}

// 资源更新通知
void ATasks::NotifyResourceUpdate(int32 ResourceIndex, const FString& ResourceName)
{
	// 更新任务进度（某些资源与任务相关）
	if (ResourceName == "TaskResource")
	{
		UpdateProgress(10); 
	}
	if (NPCActor)
	{
		UpdateNPCState();
	}
}

// 鼠标悬停显示信息
void ATasks::DisplayMouseHoverInfo(int32 ElementIndex)
{
	UpdateMouseHover(ElementIndex);
}

// 更新鼠标悬停信息
void ATasks::UpdateMouseHover(int32 ElementIndex)
{
	if (ElementIndex >= 0)
	{
		FString HoverInfo = FString::Printf(TEXT("元素 %d: 被悬停"), ElementIndex);

		// 更新 UI 上的文字
		if (HoverTextBlock)
		{
			HoverTextBlock->SetText(FText::FromString(HoverInfo));
		}

		UE_LOG(LogTemp, Log, TEXT("鼠标悬停信息已更新，元素索引: %d"), ElementIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("无效"));
	}
}

// 任务接收逻辑
void ATasks::ReceiveTask(AActor* InteractingActor)
{
	if (!InteractingActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("错误"));
		return;
	}

	NPCActor = InteractingActor;
	bIsInteracting = true;

	// 模拟任务进程的延时处理
	GetWorldTimerManager().SetTimer(TaskTimerHandle, this, &ATasks::CompleteTask, 2.0f, false);
}

// 任务完成后的逻辑
void ATasks::CompleteTask()
{
	if (bIsInteracting)
	{
		SetTaskCompleteFlag(true);
		UpdateNPCState();
		bIsInteracting = false;

		UE_LOG(LogTemp, Log, TEXT("完成"));
	}
}

// 更新 NPC 状态
void ATasks::UpdateNPCState()
{
	if (NPCActor)
	{
		UTaskStatusComponent* TaskStatus = NPCActor->FindComponentByClass<UTaskStatusComponent>();
		if (TaskStatus)
		{
			TaskStatus->SetTaskStatus(ETaskStatus::InProgress);
			UE_LOG(LogTemp, Log, TEXT("NPC 状态已更新: %s"), *NPCActor->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("错误"));
	}
}

// 设置任务完成标志
void ATasks::SetTaskCompleteFlag(bool bIsComplete)
{
	bIsTaskComplete = bIsComplete;

	if (bIsComplete)
	{
		UE_LOG(LogTemp, Log, TEXT("完成"));
		// 更新 UI 显示任务状态
		UpdateTaskUI(FText::FromString(TEXT("任务已完成！")));
		// 触发任务完成事件
		OnTaskCompleted.Broadcast();
		// 更新 NPC 状态
		if (NPCActor)
		{
			UTaskStatusComponent* TaskStatus = NPCActor->FindComponentByClass<UTaskStatusComponent>();
			if (TaskStatus)
			{
				TaskStatus->SetTaskStatus(ETaskStatus::Completed);
				UE_LOG(LogTemp, Log, TEXT("NPC 状态已更新为完成"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("未完成"));
	}
}

// 更新 UI 显示任务状态
void ATasks::UpdateTaskUI(const FText& StatusMessage)
{
	if (TaskStatusTextBlock)
	{
		TaskStatusTextBlock->SetText(StatusMessage);
		UE_LOG(LogTemp, Log, TEXT("任务状态 UI 已更新: %s"), *StatusMessage.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("任务状态 UI 未绑定"));
	}
}
// 更新任务进度
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

		UE_LOG(LogTemp, Log, TEXT("当前进度: %d%%"), CurrentProgress);
	}
}