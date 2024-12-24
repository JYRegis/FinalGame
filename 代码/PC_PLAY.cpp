#include "PC_PLAY.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Engine/LocalPlayer.h"
#include "Components/SceneComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
// 需要包含你的自定义类头文件

APC_PLAY::APC_PLAY()
{
    // 启用Tick
    PrimaryActorTick.bCanEverTick = true;

    // 初始化变量
    bIsBackpackOpen = false;
    LastTraceTime = 0.0f;
    openactor = nullptr;
    As_Bp_Build = nullptr;

    // 创建场景组件
    TransformComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TransformComponent"));
    RootComponent = TransformComponent;

    // 加载输入动作资源
    static ConstructorHelpers::FObjectFinder<UInputAction> BackpackActionAsset(TEXT("/Game/Input/IA_ToggleBackpack"));
    if (BackpackActionAsset.Succeeded())
    {
        ToggleBackpackAction = BackpackActionAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> DialogueActionAsset(TEXT("/Game/Input/IA_Dialogue"));
    if (DialogueActionAsset.Succeeded())
    {
        DialogueAction = DialogueActionAsset.Object;
    }

    // 加载背包Widget类
    static ConstructorHelpers::FClassFinder<UUserWidget> BackpackWidgetBPClass(TEXT("/Game/UI/WBP_Backpack"));
    if (BackpackWidgetBPClass.Class != nullptr)
    {
        BackpackWidgetClass = BackpackWidgetBPClass.Class;
    }
}

void APC_PLAY::BeginPlay()
{
    Super::BeginPlay();
    
    // 设置增强输入系统
    SetupEnhancedInput();
}

void APC_PLAY::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 执行交互检测
    PerformInteractionTrace(DeltaTime);
}

void APC_PLAY::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 设置增强输入组件
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // 绑定背包开关动作
        EnhancedInputComponent->BindAction(ToggleBackpackAction, ETriggerEvent::Started, this, &APC_PLAY::HandleToggleBackpack);
        
        // 绑定对话动作
        EnhancedInputComponent->BindAction(DialogueAction, ETriggerEvent::Started, this, &APC_PLAY::HandleDialogueInput);
    }
}

void APC_PLAY::SetupEnhancedInput()
{
    // 获取本地玩家的增强输入子系统
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // 确保映射上下文有效
        if (DefaultMappingContext)
        {
            // 添加映射上下文
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}
// ---------- 背包系统相关实现 ----------

void APC_PLAY::Client_OpenBp_Implementation()
{
    // 确保在客户端执行
    if (!IsLocalController())
    {
        return;
    }

    // 处理背包打开逻辑
    if (BackpackWidgetClass)
    {
        // 创建背包UI
        if (UUserWidget* BackpackWidget = CreateWidget<UUserWidget>(this, BackpackWidgetClass))
        {
            BackpackWidget->AddToViewport();
        }
    }

    // 设置输入模式为游戏和UI
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    
    // 显示鼠标光标
    bShowMouseCursor = true;
}

void APC_PLAY::FUN_OPENBp()
{
    // 在本地调用客户端RPC
    Client_OpenBp();
}

void APC_PLAY::HandleToggleBackpack()
{
    // 执行射线检测
    FHitResult HitResult;
    FVector StartLocation;
    FRotator Rotation;
    GetPlayerViewPoint(StartLocation, Rotation);
    
    FVector EndLocation = StartLocation + (Rotation.Vector() * TraceDistance);
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetPawn());
    
    bool bHasHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );

    // 如果没有击中任何物体，则切换背包状态
    if (!bHasHit)
    {
        bIsBackpackOpen = !bIsBackpackOpen;

        if (bIsBackpackOpen)
        {
            FUN_OPENBp();
        }
        else
        {
            // 关闭背包逻辑
            // 设置游戏输入模式
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);
            bShowMouseCursor = false;

            // 移除背包UI（如果需要）
            if (UGameplayStatics::GetPlayerController(GetWorld(), 0))
            {
                // 可以在这里添加关闭UI的逻辑
            }
        }
    }
}

// ---------- 位置相关实现 ----------

FVector APC_PLAY::Get2DMouseLocation()
{
    // 声明变量存储结果
    FVector WorldLocation;
    FVector WorldDirection;
    
    // 将鼠标位置从屏幕空间转换到世界空间
    if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        // 创建2D向量，忽略Z轴
        FVector2D MouseLocation2D(WorldLocation.X, WorldLocation.Y);
        
        // 返回转换后的位置，Z轴设为0
        return FVector(MouseLocation2D.X, MouseLocation2D.Y, 0.0f);
    }
    
    // 如果转换失败，返回零向量
    return FVector::ZeroVector;
}

void APC_PLAY::PerformInteractionTrace(float DeltaTime)
{
    // 更新检测计时器
    LastTraceTime += DeltaTime;
    if (LastTraceTime < TraceInterval)
    {
        return;
    }
    LastTraceTime = 0.0f;

    // 获取鼠标位置
    FVector2D MousePosition;
    if (!GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        return;
    }

    // 射线检测设置
    FVector WorldLocation, WorldDirection;
    if (DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection))
    {
        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + (WorldDirection * TraceDistance);

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(GetPawn());
        QueryParams.bTraceComplex = true;

        // 执行射线检测
        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility,
            QueryParams
        );

        // 更新鼠标光标
        UpdateMouseCursor(HitResult);
    }
}
// ---------- NPC交互相关实现 ----------

void APC_PLAY::Server_Musk_Trace_Implementation()
{
    // 确保在服务器上执行
    if (!HasAuthority())
    {
        return;
    }

    // 在这里实现服务器端的NPC交互逻辑
    if (openactor && openactor->Implements<UInteractableInterface>())
    {
        IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(openactor);
        if (InteractableInterface)
        {
            InteractableInterface->Interact(this);
        }
    }
}

bool APC_PLAY::Server_Musk_Trace_Validate()
{
    return true;
}

void APC_PLAY::FUN_Trace(ABP_BuildParent* Target, APC_PLAY* PC)
{
    // 验证参数
    if (!Target || !PC)
    {
        return;
    }

    // 存储目标引用
    openactor = Target;

    // 调用服务器RPC
    Server_Musk_Trace();
}

void APC_PLAY::HandleDialogueInput(const FInputActionValue& Value)
{
    // 获取玩家视角
    FVector StartLocation;
    FRotator Rotation;
    GetPlayerViewPoint(StartLocation, Rotation);
    
    FVector EndLocation = StartLocation + (Rotation.Vector() * TraceDistance);
    
    // 设置射线检测参数
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetPawn());
    
    FHitResult HitResult;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
    {
        // 检查是否击中了可对话的NPC
        if (ABP_BuildParent* Target = Cast<ABP_BuildParent>(HitResult.GetActor()))
        {
            if (Target->Implements<UDialogueInterface>())
            {
                IDialogueInterface* DialogueInterface = Cast<IDialogueInterface>(Target);
                if (DialogueInterface && DialogueInterface->CanStartDialogue())
                {
                    DialogueInterface->StartDialogue(this);
                }
            }
        }
    }
}

void APC_PLAY::UpdateMouseCursor(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (!HitActor)
        {
            CurrentMouseCursor = EMouseCursor::Default;
            return;
        }

        // 检查是否是可交互的NPC
        if (HitActor->Implements<UInteractableInterface>())
        {
            IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(HitActor);
            if (InteractableInterface && InteractableInterface->CanInteract())
            {
                // 设置为手型光标表示可交互
                CurrentMouseCursor = EMouseCursor::Hand;
                return;
            }
        }

        // 检查是否是可对话的NPC
        if (HitActor->Implements<UDialogueInterface>())
        {
            IDialogueInterface* DialogueInterface = Cast<IDialogueInterface>(HitActor);
            if (DialogueInterface && DialogueInterface->CanStartDialogue())
            {
                // 设置为交谈光标
                CurrentMouseCursor = EMouseCursor::TextEditBeam;
                return;
            }
        }
    }

    // 默认光标
    CurrentMouseCursor = EMouseCursor::Default;
}

#if WITH_EDITOR
void APC_PLAY::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 在编辑器中修改属性时的处理
    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
    // 可以在这里添加属性修改时的特殊处理
}
#endif
