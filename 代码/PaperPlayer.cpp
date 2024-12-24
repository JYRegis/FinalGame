// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperPlayer.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// 构造函数 
APaperPlayer::APaperPlayer()
{
    // 初始化数据成员
    CurrentTool = 0;
    XDirection = 0.0f;
    YDirection = 0.0f;
    Level = 1; // 默认等级为1
    IsFishingTool1 = false;
    IsFishingTool2 = false;
    IsFishing = false;
    IsCooking = false;

    AmmoWidgetClass = nullptr;
    FishingWidgetClass = nullptr;
    BackpackWidgetClass = nullptr;
    TaskListWidgetClass = nullptr;
}

//获取鼠标2d向量
FVector2D APaperPlayer::GetMouse2dVector()
{
	FVector3d MousePosition;
	FVector WorldDirection;

	
	Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(MousePosition, WorldDirection);

	return FVector2D (MousePosition.X, MousePosition.Y);
}
//获取鼠标2d向量并归一化
FVector2D APaperPlayer::Mouse2dVectorNormalize()
{

	FVector3d MousePosition;
	FVector WorldDirection;

    Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(MousePosition, WorldDirection);
	float x, y;
	x = FMath::TruncToFloat((((MousePosition.X + 8.0) / 16.0)))*16;
	y = FMath::TruncToFloat((((MousePosition.X + 8.0) / 16.0))) * 16;
	return FVector2D(x,y);
}
//获取人物2d向量并归一化
FVector2D APaperPlayer::Player2dVectorNormalize()
{


	
	float x, y;
	x = FMath::TruncToFloat((((GetActorLocation().X + 8.0) / 16.0))) * 16;
	y = FMath::TruncToFloat((((GetActorLocation().X + 8.0) / 16.0))) * 16;
	return FVector2D(x, y);

	
}
//绑定输入
void APaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// 移动
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaperPlayer::Move);
	}
}


//绑定输入函数
void APaperPlayer::Move(const FInputActionValue& Value)
{+
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// get forward vector
		FVector ForwardDirection = FVector (0, 1, 0);

		// get right vector 
		FVector RightDirection = FVector (1, 0, 0);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
//生成斧头逻辑
void APaperPlayer::Mouseicon()
{

	UWorld* const World = GetWorld();
	if (Experimentaltool) {
		
		FVector2D a = Player2dVectorNormalize();
		
		FVector SpawnLocation(FMath::Clamp(a.X, a.X - 32, a.X + 32), FMath::Clamp(a.Y, a.Y - 32, a.Y + 32), 10);
		FRotator SpawnRotation(0, 0, 0);
		FVector SpawnScale(1, 1, 1);
		World->SpawnActor<AActor>(BlueprintVar1, FTransform( SpawnRotation, SpawnLocation, SpawnScale));
	}
	else
	{
		FVector2D a = Player2dVectorNormalize();

		FVector SpawnLocation(FMath::Clamp(a.X, a.X - 32, a.X + 32), FMath::Clamp(a.Y, a.Y - 32, a.Y + 32), 10);
		FRotator SpawnRotation(0, 0, 0);
		FVector SpawnScale(1, 1, 1);
		World->SpawnActor<AActor>(BlueprintVar1, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
	}
}
//生成锄头逻辑
void APaperPlayer::Mousehoe()
{
	UWorld* const World = GetWorld();
	if (Experimentaltool) {

		FVector2D a = Player2dVectorNormalize();

		FVector SpawnLocation(FMath::Clamp(a.X, a.X - 32, a.X + 32), FMath::Clamp(a.Y, a.Y - 32, a.Y + 32), 10);
		FRotator SpawnRotation(0, 0, 0);
		FVector SpawnScale(1, 1, 1);
		World->SpawnActor<AActor>(BlueprintVar2, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
	}
	else
	{
		FVector2D a = Player2dVectorNormalize();

		FVector SpawnLocation(FMath::Clamp(a.X, a.X - 32, a.X + 32), FMath::Clamp(a.Y, a.Y - 32, a.Y + 32), 10);
		FRotator SpawnRotation(0, 0, 0);
		FVector SpawnScale(1, 1, 1);
		World->SpawnActor<AActor>(BlueprintVar2, FTransform(SpawnRotation, SpawnLocation, SpawnScale));
	}
}

void APaperPlayer::BeginPlay()
{
    Super::BeginPlay();

    // 调用父类的BeginPlay逻辑
    Super::BeginPlay();

    // 创建并初始化时间部件
    UWorld* World = GetWorld();
    if (World)
    {
        // 创建时间部件
        UUserWidget* TimerWidget = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
        if (TimerWidget)
        {
            TimerWidget->AddToViewport(); // 将时间部件添加到视图
        }
    }

    // 设置显示鼠标状态
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->bShowMouseCursor = true; // 显示鼠标
    }

    // 初始化玩家相关UI部件
    if (World)
    {
        // 创建弹药显示UI
        UUserWidget* AmmoWidget = CreateWidget<UUserWidget>(GetWorld(), AmmoWidgetClass);
        if (AmmoWidget)
        {
            AmmoWidget->AddToViewport(); // 添加到视图
        }

        // 创建符文显示UI
        UUserWidget* RuneWidget = CreateWidget<UUserWidget>(GetWorld(), RuneWidgetClass);
        if (RuneWidget)
        {
            RuneWidget->AddToViewport(); // 添加到视图
        }

        // 创建背包显示UI
        UUserWidget* BackpackWidget = CreateWidget<UUserWidget>(GetWorld(), BackpackWidgetClass);
        if (BackpackWidget)
        {
            BackpackWidget->AddToViewport(); // 添加到视图
        }
    }
}

void APaperPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 移动绑定
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaperPlayer::Move);

        // 添加按键绑定逻辑
        // 绑定工具切换按键 (按键1)
        EnhancedInputComponent->BindAction("ToolKey1", ETriggerEvent::Triggered, this, &APaperPlayer::SwitchTool1);

        // 绑定工具切换按键 (按键2)
        EnhancedInputComponent->BindAction("ToolKey2", ETriggerEvent::Triggered, this, &APaperPlayer::SwitchTool2);

        // 绑定工具切换按键 (按键3)
        EnhancedInputComponent->BindAction("ToolKey3", ETriggerEvent::Triggered, this, &APaperPlayer::SwitchTool3);

        // 打开背包
        EnhancedInputComponent->BindAction("OpenBackpack", ETriggerEvent::Triggered, this, &APaperPlayer::OpenBackpack);

        // 绑定伐木工具按键 Z
        EnhancedInputComponent->BindAction("CutWood", ETriggerEvent::Triggered, this, &APaperPlayer::ToggleWoodTool);

        // 绑定采矿工具按键 X
        EnhancedInputComponent->BindAction("MineOre", ETriggerEvent::Triggered, this, &APaperPlayer::ToggleMiningTool);

        // 绑定暂停按键 B (伐木)
        EnhancedInputComponent->BindAction("PauseWood", ETriggerEvent::Triggered, this, &APaperPlayer::PauseWoodAction);

        // 绑定暂停按键 N (采矿)
        EnhancedInputComponent->BindAction("PauseOre", ETriggerEvent::Triggered, this, &APaperPlayer::PauseOreAction);
    }
}

// 切换工具1逻辑
void APaperPlayer::SwitchTool1()
{
    // 更新当前工具为工具斧子 
    CurrentTool = BlueprintVar1;
}

// 切换工具2逻辑
void APaperPlayer::SwitchTool2()
{
    // 更新当前工具为工具镐子
    CurrentTool = BlueprintVar2;
}

// 切换工具3逻辑
void APaperPlayer::SwitchTool3()
{
    // 更新当前工具为工具鱼竿
    CurrentTool = BlueprintVar3;
}

// 打开背包逻辑
void APaperPlayer::OpenBackpack()
{
    // 显示背包界面
    if (BackpackWidget)
    {
        BackpackWidget->AddToViewport();
    }
}

// 切换伐木工具
void APaperPlayer::ToggleWoodTool()
{
    bWoodTool = !bWoodTool;
}

// 切换采矿工具
void APaperPlayer::ToggleMiningTool()
{
    bMiningTool = !bMiningTool;
}

// 暂停伐木行为
void APaperPlayer::PauseWoodAction()
{
    // 设置暂停状态并隐藏鼠标
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->SetPause(!PC->IsPaused());
        PC->bShowMouseCursor = !PC->IsPaused();
    }
}

// 暂停采矿行为
void APaperPlayer::PauseOreAction()
{
    // 设置暂停状态并隐藏鼠标
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->SetPause(!PC->IsPaused());
        PC->bShowMouseCursor = !PC->IsPaused();
    }
}
