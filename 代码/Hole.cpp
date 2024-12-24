#include "Hole.h"
#include "TimerManager.h"

// 构造函数
AHole::AHole()
{
	// 启用 Tick
	PrimaryActorTick.bCanEverTick = true;

	// 初始化默认值
	HoleState = TEXT("Default");
	bIsVisible = true;

	// 初始化新增变量
	bIsPlanted = false;        // 默认未种植
	GrowthDuration = 0;        // 默认生长时长为 0
	bCanBeHarvested = false;   // 默认不可收获
	RowName = FName(TEXT("")); // 默认行名称为空
	SeedType = TEXT("");       // 默认种子类型为空
	CategoryType = TEXT("");   // 默认类别类型为空
}

// 收获事件
void AHole::Harvest()
{
    // 检查是否可收获
    if (!bCanBeHarvested)
    {
        return;
    }

    // 获取当前Actor的位置
    FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 2.0f);
    FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
    FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

    // 生成Actor
    if (SpawnedActorClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnedActorClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedActor)
        {
            // 设置Actor的类别和行名称
            FName NewRowName = RowName;  // 行名称
            FString NewCategoryType = CategoryType;  // 类别

        }
    }

    // 更新Hole的状态
    bCanBeHarvested = false;
    bIsPlanted = false;
    GrowthDuration = 10; // 重置生长时长为10
}

void AHole::PlantSeed(const FString& SeedType, int32 GrowthStage)
{
    // 设置种类和生长阶段
    this->SeedType = SeedType;
    this->SeedGrowthStage = GrowthStage;

    // 检查种子是否有效
    if (!SeedType.IsEmpty())
    {
        UpdateSeedSprite();
        bIsPlanted = true;
        SetVisibility(true);
    }
}

// 更新种子外观
void AHole::UpdateSeedSprite()
{
    // 根据种子类型和生长阶段选择Sprite（假设有逻辑或表可以映射到正确的Sprite）
    if (SeedType == "Tool")
    {
        CurrentSeedSprite = CSprite.Sprite;
    }
    else if (SeedType == "Paper")
    {
        CurrentSeedSprite = CSprite.Seed;
    }
    else
    {
        return;
    }

    if (CurrentSeedSprite)
    {
        UPaperSpriteComponent* SeedSpriteComponent = FindComponentByClass<UPaperSpriteComponent>();
        if (SeedSpriteComponent)
        {
            SeedSpriteComponent->SetSprite(CurrentSeedSprite);
            SeedSpriteComponent->SetVisibility(true);
        }
    }
}

// 浇水事件
void AHole::WaterCrop()
{
    // 检查是否已经种植
    if (!bIsPlanted)
    {
        return;
    }

    // 检查当前生长阶段
    if (SeedGrowthStage >= MaxGrowthStage)
    {
        return;
    }

    // 启动定时器以更新作物生长
    GetWorldTimerManager().SetTimer(GrowthTimerHandle, this, &AHole::UpdateCropGrowth, GrowthDurationPerStage, true);

    UE_LOG(LogTemp, Log, TEXT("Watering crop... Growth process started."));
}

// 作物生长事件
void AHole::UpdateCropGrowth()
{
    // 增加生长阶段
    if (SeedGrowthStage < MaxGrowthStage)
    {
        this->SeedGrowthStage++;

        // 更新种子的Sprite外观
        UpdateSeedSprite();

        // 如果达到最大生长阶段，设置为可收获
        if (SeedGrowthStage >= MaxGrowthStage)
        {
            bCanBeHarvested = true;
            GetWorldTimerManager().ClearTimer(GrowthTimerHandle); // 停止定时器
        }
    }
}

// 游戏开始时调用
void AHole::BeginPlay()
{
	Super::BeginPlay();

	UpdateVisibility();
}

// 每帧调用
void AHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 可扩展的逻辑处理
}

// 设置 Hole 可见性
void AHole::SetVisibility(bool bVisible)
{
	bIsVisible = bVisible;
	UpdateVisibility();
}

// 内部更新 Hole 可见性
void AHole::UpdateVisibility()
{
	SetActorHiddenInGame(!bIsVisible);
}

// 设置 Hole 的状态
void AHole::SetHoleState(const FString& State)
{
	HoleState = State;
	UE_LOG(LogTemp, Log, TEXT("Hole state set to: %s"), *State);

	// 根据状态修改 Hole 外观或功能
	if (HoleState == "Empty")
	{
		// 更新外观
		SetVisibility(true);
	}
	else if (HoleState == "Filled")
	{
		SetVisibility(false);
	}
}

// 启动定时器
void AHole::StartTimer(float Duration)
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AHole::OnTimerCompleted, Duration, false);
}

// 定时器完成时回调
void AHole::OnTimerCompleted()
{
	SetHoleState("Default");
}


