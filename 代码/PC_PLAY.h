#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PC_PLAY.generated.h"

// 前向声明
class UBP_BuildParent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class YOURPROJECT_API APC_PLAY : public APlayerController
{
    GENERATED_BODY()

public:
    APC_PLAY();

    // 重写函数
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // ---------- 背包系统相关 ----------
    /** 客户端RPC - 打开背包 */
    UFUNCTION(Client, Reliable)
    void Client_OpenBp();

    /** 供蓝图调用的打开背包函数 */
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void FUN_OPENBp();

    // ---------- 鼠标位置相关 ----------
    /** 获取鼠标2D位置 */
    UFUNCTION(BlueprintCallable, Category = "Mouse")
    FVector Get2DMouseLocation();

    // ---------- NPC交互相关 ----------
    /** 服务器RPC - 与Musk交互 */
    UFUNCTION(Server, Reliable)
    void Server_Musk_Trace();

    /** 客户端调用的交互函数 */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void FUN_Trace(class ABP_BuildParent* Target, APC_PLAY* PC);

protected:
    // ---------- 输入相关变量 ----------
    /** 默认输入映射上下文 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    /** 背包开关动作 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* ToggleBackpackAction;

    /** 对话动作 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* DialogueAction;

    // ---------- 背包系统变量 ----------
    /** 背包是否打开 */
    UPROPERTY()
    bool bIsBackpackOpen;

    /** 背包Widget类 */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> BackpackWidgetClass;

    // ---------- NPC交互变量 ----------
    /** 存储当前交互的NPC */
    UPROPERTY()
    class ABP_BuildParent* openactor;

    /** 存储位置向量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FVector NewVar;

    /** BP Build父类引用 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    class ABP_BuildParent* As_Bp_Build;

private:
    // ---------- 组件 ----------
    /** 场景组件 */
    UPROPERTY(VisibleAnywhere)
    class USceneComponent* TransformComponent;

    // ---------- 内部函数 ----------
    /** 设置增强输入系统 */
    void SetupEnhancedInput();
    
    /** 处理背包开关 */
    UFUNCTION()
    void HandleToggleBackpack();
    
    /** 处理对话输入 */
    UFUNCTION()
    void HandleDialogueInput(const FInputActionValue& Value);
    
    /** 执行交互检测 */
    void PerformInteractionTrace(float DeltaTime);
    
    /** 更新鼠标光标 */
    void UpdateMouseCursor(const FHitResult& HitResult);

    // ---------- 射线检测相关 ----------
    /** 射线检测距离 */
    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    float TraceDistance = 200.0f;

    /** 射线检测间隔 */
    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    float TraceInterval = 0.1f;

    /** 上次检测时间 */
    float LastTraceTime;
};
