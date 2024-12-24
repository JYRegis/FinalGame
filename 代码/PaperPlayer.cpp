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

// ���캯�� 
APaperPlayer::APaperPlayer()
{
    // ��ʼ�����ݳ�Ա
    CurrentTool = 0;
    XDirection = 0.0f;
    YDirection = 0.0f;
    Level = 1; // Ĭ�ϵȼ�Ϊ1
    IsFishingTool1 = false;
    IsFishingTool2 = false;
    IsFishing = false;
    IsCooking = false;

    AmmoWidgetClass = nullptr;
    FishingWidgetClass = nullptr;
    BackpackWidgetClass = nullptr;
    TaskListWidgetClass = nullptr;
}

//��ȡ���2d����
FVector2D APaperPlayer::GetMouse2dVector()
{
	FVector3d MousePosition;
	FVector WorldDirection;

	
	Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(MousePosition, WorldDirection);

	return FVector2D (MousePosition.X, MousePosition.Y);
}
//��ȡ���2d��������һ��
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
//��ȡ����2d��������һ��
FVector2D APaperPlayer::Player2dVectorNormalize()
{


	
	float x, y;
	x = FMath::TruncToFloat((((GetActorLocation().X + 8.0) / 16.0))) * 16;
	y = FMath::TruncToFloat((((GetActorLocation().X + 8.0) / 16.0))) * 16;
	return FVector2D(x, y);

	
}
//������
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
		// �ƶ�
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaperPlayer::Move);
	}
}


//�����뺯��
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
//���ɸ�ͷ�߼�
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
//���ɳ�ͷ�߼�
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

    // ���ø����BeginPlay�߼�
    Super::BeginPlay();

    // ��������ʼ��ʱ�䲿��
    UWorld* World = GetWorld();
    if (World)
    {
        // ����ʱ�䲿��
        UUserWidget* TimerWidget = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
        if (TimerWidget)
        {
            TimerWidget->AddToViewport(); // ��ʱ�䲿����ӵ���ͼ
        }
    }

    // ������ʾ���״̬
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->bShowMouseCursor = true; // ��ʾ���
    }

    // ��ʼ��������UI����
    if (World)
    {
        // ������ҩ��ʾUI
        UUserWidget* AmmoWidget = CreateWidget<UUserWidget>(GetWorld(), AmmoWidgetClass);
        if (AmmoWidget)
        {
            AmmoWidget->AddToViewport(); // ��ӵ���ͼ
        }

        // ����������ʾUI
        UUserWidget* RuneWidget = CreateWidget<UUserWidget>(GetWorld(), RuneWidgetClass);
        if (RuneWidget)
        {
            RuneWidget->AddToViewport(); // ��ӵ���ͼ
        }

        // ����������ʾUI
        UUserWidget* BackpackWidget = CreateWidget<UUserWidget>(GetWorld(), BackpackWidgetClass);
        if (BackpackWidget)
        {
            BackpackWidget->AddToViewport(); // ��ӵ���ͼ
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
        // �ƶ���
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaperPlayer::Move);

        // ��Ӱ������߼�
        // �󶨹����л����� (����1)
        EnhancedInputComponent->BindAction("ToolKey1", ETriggerEvent::Triggered, this, &APaperPlayer::SwitchTool1);

        // �󶨹����л����� (����2)
        EnhancedInputComponent->BindAction("ToolKey2", ETriggerEvent::Triggered, this, &APaperPlayer::SwitchTool2);

        // �󶨹����л����� (����3)
        EnhancedInputComponent->BindAction("ToolKey3", ETriggerEvent::Triggered, this, &APaperPlayer::SwitchTool3);

        // �򿪱���
        EnhancedInputComponent->BindAction("OpenBackpack", ETriggerEvent::Triggered, this, &APaperPlayer::OpenBackpack);

        // �󶨷�ľ���߰��� Z
        EnhancedInputComponent->BindAction("CutWood", ETriggerEvent::Triggered, this, &APaperPlayer::ToggleWoodTool);

        // �󶨲ɿ󹤾߰��� X
        EnhancedInputComponent->BindAction("MineOre", ETriggerEvent::Triggered, this, &APaperPlayer::ToggleMiningTool);

        // ����ͣ���� B (��ľ)
        EnhancedInputComponent->BindAction("PauseWood", ETriggerEvent::Triggered, this, &APaperPlayer::PauseWoodAction);

        // ����ͣ���� N (�ɿ�)
        EnhancedInputComponent->BindAction("PauseOre", ETriggerEvent::Triggered, this, &APaperPlayer::PauseOreAction);
    }
}

// �л�����1�߼�
void APaperPlayer::SwitchTool1()
{
    // ���µ�ǰ����Ϊ���߸��� 
    CurrentTool = BlueprintVar1;
}

// �л�����2�߼�
void APaperPlayer::SwitchTool2()
{
    // ���µ�ǰ����Ϊ���߸���
    CurrentTool = BlueprintVar2;
}

// �л�����3�߼�
void APaperPlayer::SwitchTool3()
{
    // ���µ�ǰ����Ϊ�������
    CurrentTool = BlueprintVar3;
}

// �򿪱����߼�
void APaperPlayer::OpenBackpack()
{
    // ��ʾ��������
    if (BackpackWidget)
    {
        BackpackWidget->AddToViewport();
    }
}

// �л���ľ����
void APaperPlayer::ToggleWoodTool()
{
    bWoodTool = !bWoodTool;
}

// �л��ɿ󹤾�
void APaperPlayer::ToggleMiningTool()
{
    bMiningTool = !bMiningTool;
}

// ��ͣ��ľ��Ϊ
void APaperPlayer::PauseWoodAction()
{
    // ������ͣ״̬���������
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->SetPause(!PC->IsPaused());
        PC->bShowMouseCursor = !PC->IsPaused();
    }
}

// ��ͣ�ɿ���Ϊ
void APaperPlayer::PauseOreAction()
{
    // ������ͣ״̬���������
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->SetPause(!PC->IsPaused());
        PC->bShowMouseCursor = !PC->IsPaused();
    }
}
