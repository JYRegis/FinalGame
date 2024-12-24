#include "PC_PLAY.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Engine/LocalPlayer.h"
#include "Components/SceneComponent.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
// ��Ҫ��������Զ�����ͷ�ļ�

APC_PLAY::APC_PLAY()
{
    // ����Tick
    PrimaryActorTick.bCanEverTick = true;

    // ��ʼ������
    bIsBackpackOpen = false;
    LastTraceTime = 0.0f;
    openactor = nullptr;
    As_Bp_Build = nullptr;

    // �����������
    TransformComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TransformComponent"));
    RootComponent = TransformComponent;

    // �������붯����Դ
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

    // ���ر���Widget��
    static ConstructorHelpers::FClassFinder<UUserWidget> BackpackWidgetBPClass(TEXT("/Game/UI/WBP_Backpack"));
    if (BackpackWidgetBPClass.Class != nullptr)
    {
        BackpackWidgetClass = BackpackWidgetBPClass.Class;
    }
}

void APC_PLAY::BeginPlay()
{
    Super::BeginPlay();
    
    // ������ǿ����ϵͳ
    SetupEnhancedInput();
}

void APC_PLAY::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // ִ�н������
    PerformInteractionTrace(DeltaTime);
}

void APC_PLAY::SetupInputComponent()
{
    Super::SetupInputComponent();

    // ������ǿ�������
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // �󶨱������ض���
        EnhancedInputComponent->BindAction(ToggleBackpackAction, ETriggerEvent::Started, this, &APC_PLAY::HandleToggleBackpack);
        
        // �󶨶Ի�����
        EnhancedInputComponent->BindAction(DialogueAction, ETriggerEvent::Started, this, &APC_PLAY::HandleDialogueInput);
    }
}

void APC_PLAY::SetupEnhancedInput()
{
    // ��ȡ������ҵ���ǿ������ϵͳ
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // ȷ��ӳ����������Ч
        if (DefaultMappingContext)
        {
            // ���ӳ��������
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}
// ---------- ����ϵͳ���ʵ�� ----------

void APC_PLAY::Client_OpenBp_Implementation()
{
    // ȷ���ڿͻ���ִ��
    if (!IsLocalController())
    {
        return;
    }

    // ���������߼�
    if (BackpackWidgetClass)
    {
        // ��������UI
        if (UUserWidget* BackpackWidget = CreateWidget<UUserWidget>(this, BackpackWidgetClass))
        {
            BackpackWidget->AddToViewport();
        }
    }

    // ��������ģʽΪ��Ϸ��UI
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    
    // ��ʾ�����
    bShowMouseCursor = true;
}

void APC_PLAY::FUN_OPENBp()
{
    // �ڱ��ص��ÿͻ���RPC
    Client_OpenBp();
}

void APC_PLAY::HandleToggleBackpack()
{
    // ִ�����߼��
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

    // ���û�л����κ����壬���л�����״̬
    if (!bHasHit)
    {
        bIsBackpackOpen = !bIsBackpackOpen;

        if (bIsBackpackOpen)
        {
            FUN_OPENBp();
        }
        else
        {
            // �رձ����߼�
            // ������Ϸ����ģʽ
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);
            bShowMouseCursor = false;

            // �Ƴ�����UI�������Ҫ��
            if (UGameplayStatics::GetPlayerController(GetWorld(), 0))
            {
                // ������������ӹر�UI���߼�
            }
        }
    }
}

// ---------- λ�����ʵ�� ----------

FVector APC_PLAY::Get2DMouseLocation()
{
    // ���������洢���
    FVector WorldLocation;
    FVector WorldDirection;
    
    // �����λ�ô���Ļ�ռ�ת��������ռ�
    if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        // ����2D����������Z��
        FVector2D MouseLocation2D(WorldLocation.X, WorldLocation.Y);
        
        // ����ת�����λ�ã�Z����Ϊ0
        return FVector(MouseLocation2D.X, MouseLocation2D.Y, 0.0f);
    }
    
    // ���ת��ʧ�ܣ�����������
    return FVector::ZeroVector;
}

void APC_PLAY::PerformInteractionTrace(float DeltaTime)
{
    // ���¼���ʱ��
    LastTraceTime += DeltaTime;
    if (LastTraceTime < TraceInterval)
    {
        return;
    }
    LastTraceTime = 0.0f;

    // ��ȡ���λ��
    FVector2D MousePosition;
    if (!GetMousePosition(MousePosition.X, MousePosition.Y))
    {
        return;
    }

    // ���߼������
    FVector WorldLocation, WorldDirection;
    if (DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection))
    {
        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + (WorldDirection * TraceDistance);

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(GetPawn());
        QueryParams.bTraceComplex = true;

        // ִ�����߼��
        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility,
            QueryParams
        );

        // ���������
        UpdateMouseCursor(HitResult);
    }
}
// ---------- NPC�������ʵ�� ----------

void APC_PLAY::Server_Musk_Trace_Implementation()
{
    // ȷ���ڷ�������ִ��
    if (!HasAuthority())
    {
        return;
    }

    // ������ʵ�ַ������˵�NPC�����߼�
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
    // ��֤����
    if (!Target || !PC)
    {
        return;
    }

    // �洢Ŀ������
    openactor = Target;

    // ���÷�����RPC
    Server_Musk_Trace();
}

void APC_PLAY::HandleDialogueInput(const FInputActionValue& Value)
{
    // ��ȡ����ӽ�
    FVector StartLocation;
    FRotator Rotation;
    GetPlayerViewPoint(StartLocation, Rotation);
    
    FVector EndLocation = StartLocation + (Rotation.Vector() * TraceDistance);
    
    // �������߼�����
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetPawn());
    
    FHitResult HitResult;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
    {
        // ����Ƿ�����˿ɶԻ���NPC
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

        // ����Ƿ��ǿɽ�����NPC
        if (HitActor->Implements<UInteractableInterface>())
        {
            IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(HitActor);
            if (InteractableInterface && InteractableInterface->CanInteract())
            {
                // ����Ϊ���͹���ʾ�ɽ���
                CurrentMouseCursor = EMouseCursor::Hand;
                return;
            }
        }

        // ����Ƿ��ǿɶԻ���NPC
        if (HitActor->Implements<UDialogueInterface>())
        {
            IDialogueInterface* DialogueInterface = Cast<IDialogueInterface>(HitActor);
            if (DialogueInterface && DialogueInterface->CanStartDialogue())
            {
                // ����Ϊ��̸���
                CurrentMouseCursor = EMouseCursor::TextEditBeam;
                return;
            }
        }
    }

    // Ĭ�Ϲ��
    CurrentMouseCursor = EMouseCursor::Default;
}

#if WITH_EDITOR
void APC_PLAY::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // �ڱ༭�����޸�����ʱ�Ĵ���
    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
    // ������������������޸�ʱ�����⴦��
}
#endif
