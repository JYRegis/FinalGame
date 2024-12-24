#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tasks.generated.h"

UCLASS()
class LEARN_2D_01_API ATasks : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	ATasks();

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

public:
	// 每帧调用
	virtual void Tick(float DeltaTime) override;

	// 资源更新通知
	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void NotifyResourceUpdate(int32 ResourceIndex, const FString& ResourceName);

	// 鼠标悬停显示信息
	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void DisplayMouseHoverInfo(int32 ElementIndex);

	// 任务接收逻辑
	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void ReceiveTask(AActor* InteractingActor);

private:
	// NPC 引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	AActor* NPCActor;

	// 是否正在交互
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	bool bIsInteracting;

	//是否完成
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	bool bIsTaskComplete;

	// 任务进度变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	int32 CurrentProgress;
	// 任务进度变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	int32 = RequiredProgress;

	// 存储每个元素的描述信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	TMap<int32, FString> ElementDescriptions; 

	// 存储每个元素对应的 Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	TMap<int32, AActor*> ElementActors;

	// 存储每个元素对应的 Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tasks", meta = (AllowPrivateAccess = "true"))
	FOnTaskCompleted OnTaskCompleted;

	// 定时器句柄，用于任务交互逻辑
	FTimerHandle TaskTimerHandle;

	// 任务完成后的逻辑
	void CompleteTask();

	// 更新 NPC 状态
	void UpdateNPCState();

	// 更新鼠标悬停信息的逻辑
	void UpdateMouseHover(int32 ElementIndex);

	// 设置任务完成标志
	void SetTaskCompleteFlag(bool bIsComplete);

	// 更新任务进度
	void UpdateProgress(int32 ProgressAmount);

	// 更新 UI 显示任务状态
	void UpdateTaskUI(const FText& StatusMessage);

};
