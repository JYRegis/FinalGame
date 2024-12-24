#include "Fruit.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"

// 构造函数
AFruit::AFruit()
{
	// 启用 Tick
	PrimaryActorTick.bCanEverTick = true;

	// 初始化 FruitBox
	FruitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FruitBox"));
	FruitBox->SetupAttachment(RootComponent);

	// 初始化 FruitSprite
	FruitSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FruitSprite"));
	FruitSprite->SetupAttachment(FruitBox);
	
	// 默认跳动动画参数
    JumpHeight = 150.0f; // 跳动高度
    JumpDuration = 1.5f; // 跳动时长
    bShouldMove = false; // 默认不移动
    MovementSpeed = 100.0f; // 移动速度
}

// 游戏开始时调用
void AFruit::BeginPlay()
{
	Super::BeginPlay();
	// 记录初始位置
    InitialLocation = GetActorLocation();

    // 设置初始可见性
    SetFruitVisibility(true);
     
    // 初始化时间轴
    if (JumpCurve)
    {
        // 设置时间轴回调
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("HandleJumpProgress"));
        JumpTimeline.AddInterpFloat(JumpCurve, ProgressFunction);

        // 设置时间轴完成回调
        FOnTimelineEvent FinishFunction;
        FinishFunction.BindUFunction(this, FName("OnJumpFinished"));
        JumpTimeline.SetTimelineFinishedFunc(FinishFunction);

        // 设置时间轴属性
        JumpTimeline.SetLooping(false);
        JumpTimeline.SetPlayRate(1.0f / JumpDuration);

        // 开始播放时间轴
        JumpTimeline.PlayFromStart();
    }

    // 初始化碰撞组件
    if (FruitBox)
    {
        FruitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        FruitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
        FruitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        FruitBox->OnComponentBeginOverlap.AddDynamic(this, &AFruit::OnFruitOverlap);
    }
}

// 每帧调用
void AFruit::Tick(float DeltaTime)
{
    Super::Tick();

    // 更新时间轴
    if (JumpTimeline.IsPlaying())
    {
        JumpTimeline.TickTimeline(DeltaTime);
    }

    // 跳动结束后，停止更新
    if (!JumpTimeline.IsPlaying() && bShouldMove)
    {
        // 跳动结束后，执行其他逻辑
        bShouldMove = false;  // 停止任何后续移动
    }
}


// 控制跳动的进度
void AFruit::HandleJumpProgress(float Value)
{
    // 计算跳动的当前位置
    FVector NewLocation = InitialLocation;
    NewLocation.Z += Value * JumpHeight;

    // 更新水果的位置
    SetActorLocation(NewLocation);
}

// 跳动完成时的回调
void AFruit::OnJumpFinished()
{
    // 跳动完成后停止跳动
    bShouldMove = false;
}

//切换类型 
void AItems::SwitchType(const FString& Selection)
{
    if (DataTable && !Selection.IsEmpty())
    {
        // 根据传入的 Selection 查找数据表中的对应行
        FItemData* ItemData = DataTable->FindRow<FItemData>(FName(*Selection), TEXT(""));

        if (ItemData)
        {
            // 更新当前行的 RowName，以便 UpdateSprite 可以基于这个更新精灵
            RowName = FName(*Selection);

            // 更新物品的精灵
            UpdateSprite();
        }
    }
}

// 控制水果移动动画
void AFruit::MoveFruit(const FVector& Direction, float Speed)
{
	if (FruitBox)
	{
		FVector NewLocation = GetActorLocation() + (Direction * Speed);
		SetActorLocation(NewLocation);
	}
}
// 设置水果可见性
void AFruit::SetFruitVisibility(bool bVisible)
{
    if (FruitSprite)
    {
        FruitSprite->SetVisibility(bVisible, true);

        // 如果不可见，则禁用碰撞
        if (FruitBox)
        {
            FruitBox->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
        }
    }
}

// 检查水果是否有效
bool AFruit::IsValidFruit() const
{
    return FruitSprite != nullptr && FruitBox != nullptr;
}

//收获果实 
void AFruit::OnFruitCollected()
{
    // 隐藏水果并销毁
    SetFruitVisibility(false);
    SetLifeSpan(1.0f); // 延迟销毁
}

//更新 Sprite
void AItems::UpdateSprite()
{
    // 根据行名称查找数据表中的行
    FItemData* ItemData = DataTable->FindRow<FItemData>(RowName, TEXT(""));

    if (ItemData)
    {
        // 更新物品的精灵
        PaperSprite->SetSprite(ItemData->Sprite);
    }
}

