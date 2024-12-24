#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PC_PLAY.generated.h"

// ǰ������
class UBP_BuildParent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class YOURPROJECT_API APC_PLAY : public APlayerController
{
    GENERATED_BODY()

public:
    APC_PLAY();

    // ��д����
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // ---------- ����ϵͳ��� ----------
    /** �ͻ���RPC - �򿪱��� */
    UFUNCTION(Client, Reliable)
    void Client_OpenBp();

    /** ����ͼ���õĴ򿪱������� */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void FUN_OPENBp();

    // ---------- ���λ����� ----------
    /** ��ȡ���2Dλ�� */
    UFUNCTION(BlueprintCallable, Category = "Mouse")
    FVector Get2DMouseLocation();

    // ---------- NPC������� ----------
    /** ������RPC - ��Musk���� */
    UFUNCTION(Server, Reliable)
    void Server_Musk_Trace();

    /** �ͻ��˵��õĽ������� */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void FUN_Trace(class ABP_BuildParent* Target, APC_PLAY* PC);

protected:
    // ---------- ������ر��� ----------
    /** Ĭ������ӳ�������� */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    /** �������ض��� */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* ToggleBackpackAction;

    /** �Ի����� */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* DialogueAction;

    // ---------- ����ϵͳ���� ----------
    /** �����Ƿ�� */
    UPROPERTY()
    bool bIsBackpackOpen;

    /** ����Widget�� */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> BackpackWidgetClass;

    // ---------- NPC�������� ----------
    /** �洢��ǰ������NPC */
    UPROPERTY()
    class ABP_BuildParent* openactor;

    /** �洢λ������ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FVector NewVar;

    /** BP Build�������� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    class ABP_BuildParent* As_Bp_Build;

private:
    // ---------- ��� ----------
    /** ������� */
    UPROPERTY(VisibleAnywhere)
    class USceneComponent* TransformComponent;

    // ---------- �ڲ����� ----------
    /** ������ǿ����ϵͳ */
    void SetupEnhancedInput();
    
    /** ���������� */
    UFUNCTION()
    void HandleToggleBackpack();
    
    /** ����Ի����� */
    UFUNCTION()
    void HandleDialogueInput(const FInputActionValue& Value);
    
    /** ִ�н������ */
    void PerformInteractionTrace(float DeltaTime);
    
    /** ��������� */
    void UpdateMouseCursor(const FHitResult& HitResult);

    // ---------- ���߼����� ----------
    /** ���߼����� */
    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    float TraceDistance = 200.0f;

    /** ���߼���� */
    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    float TraceInterval = 0.1f;

    /** �ϴμ��ʱ�� */
    float LastTraceTime;
};
