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
	int32 CurrentTool; // 当前工具编号

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	float XDirection; // X方向的浮点值

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	float YDirection; // Y方向的浮点值

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	int32 Level; // 等级

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsFishingTool1; // 可交互箭头1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsFishingTool2; // 可交互箭头2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsFishing; // 是否可以交互（钓鱼）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> AmmoWidgetClass; // 弹药窗口类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> FishingWidgetClass; // 钓鱼UI类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> BackpackWidgetClass; // 背包UI类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool IsCooking; // 是否处于做饭状态

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> TaskListWidgetClass; // 任务列表控件类
public:

	// 获取鼠标2D向量
	UFUNCTION(BlueprintPure)
	FVector2D GetMouse2dVector();

	// 获取鼠标2D向量并归一化
	UFUNCTION(BlueprintPure)
	FVector2D Mouse2dVectorNormalize();

	// 获取人物2D向量并归一化
	UFUNCTION(BlueprintPure)
	FVector2D Player2dVectorNormalize();

	// 绑定输入
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 移动逻辑
	void Move(const FInputActionValue& Value);

	// 工具切换逻辑
	void Mouseicon();
	void Mousehoe();

	// 新增功能方法声明
	void SwitchTool1(); // 切换工具1
	void SwitchTool2(); // 切换工具2
	void SwitchTool3(); // 切换工具3
	void OpenBackpack(); // 打开背包
	void ToggleWoodTool(); // 切换伐木工具
	void ToggleMiningTool(); // 切换采矿工具
	void PauseWoodAction(); // 暂停伐木行为
	void PauseOreAction(); // 暂停采矿行为

	// 输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Experimentaltool;

	// 获取锄头
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BlueprintVar1;

	// 获取斧头
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BlueprintVar2;

	// 新增成员变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BlueprintVar3; // 工具3

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> BackpackWidgetClass; // 背包界面类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* BackpackWidget; // 背包实例

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool bWoodTool; // 伐木工具状态

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	bool bMiningTool; // 采矿工具状态

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> TimerWidgetClass; // 时间部件类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* TimerWidget; // 时间部件实例

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> AmmoWidgetClass; // 弹药显示UI类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* AmmoWidget; // 弹药显示UI实例

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> RuneWidgetClass; // 符文显示UI类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* RuneWidget; // 符文显示UI实例

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<UUserWidget> BackpackWidgetClass; // 背包显示UI类

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	UUserWidget* BackpackWidget; // 背包显示UI实例

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<AActor> CurrentTool; // 当前工具
};

