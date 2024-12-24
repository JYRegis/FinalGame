#include "Hole.h"
#include "TimerManager.h"

// ���캯��
AHole::AHole()
{
	// ���� Tick
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ��Ĭ��ֵ
	HoleState = TEXT("Default");
	bIsVisible = true;

	// ��ʼ����������
	bIsPlanted = false;        // Ĭ��δ��ֲ
	GrowthDuration = 0;        // Ĭ������ʱ��Ϊ 0
	bCanBeHarvested = false;   // Ĭ�ϲ����ջ�
	RowName = FName(TEXT("")); // Ĭ��������Ϊ��
	SeedType = TEXT("");       // Ĭ����������Ϊ��
	CategoryType = TEXT("");   // Ĭ���������Ϊ��
}

// �ջ��¼�
void AHole::Harvest()
{
    // ����Ƿ���ջ�
    if (!bCanBeHarvested)
    {
        return;
    }

    // ��ȡ��ǰActor��λ��
    FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 2.0f);
    FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
    FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

    // ����Actor
    if (SpawnedActorClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnedActorClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (SpawnedActor)
        {
            // ����Actor������������
            FName NewRowName = RowName;  // ������
            FString NewCategoryType = CategoryType;  // ���

        }
    }

    // ����Hole��״̬
    bCanBeHarvested = false;
    bIsPlanted = false;
    GrowthDuration = 10; // ��������ʱ��Ϊ10
}

void AHole::PlantSeed(const FString& SeedType, int32 GrowthStage)
{
    // ��������������׶�
    this->SeedType = SeedType;
    this->SeedGrowthStage = GrowthStage;

    // ��������Ƿ���Ч
    if (!SeedType.IsEmpty())
    {
        UpdateSeedSprite();
        bIsPlanted = true;
        SetVisibility(true);
    }
}

// �����������
void AHole::UpdateSeedSprite()
{
    // �����������ͺ������׶�ѡ��Sprite���������߼�������ӳ�䵽��ȷ��Sprite��
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

// ��ˮ�¼�
void AHole::WaterCrop()
{
    // ����Ƿ��Ѿ���ֲ
    if (!bIsPlanted)
    {
        return;
    }

    // ��鵱ǰ�����׶�
    if (SeedGrowthStage >= MaxGrowthStage)
    {
        return;
    }

    // ������ʱ���Ը�����������
    GetWorldTimerManager().SetTimer(GrowthTimerHandle, this, &AHole::UpdateCropGrowth, GrowthDurationPerStage, true);

    UE_LOG(LogTemp, Log, TEXT("Watering crop... Growth process started."));
}

// ���������¼�
void AHole::UpdateCropGrowth()
{
    // ���������׶�
    if (SeedGrowthStage < MaxGrowthStage)
    {
        this->SeedGrowthStage++;

        // �������ӵ�Sprite���
        UpdateSeedSprite();

        // ����ﵽ��������׶Σ�����Ϊ���ջ�
        if (SeedGrowthStage >= MaxGrowthStage)
        {
            bCanBeHarvested = true;
            GetWorldTimerManager().ClearTimer(GrowthTimerHandle); // ֹͣ��ʱ��
        }
    }
}

// ��Ϸ��ʼʱ����
void AHole::BeginPlay()
{
	Super::BeginPlay();

	UpdateVisibility();
}

// ÿ֡����
void AHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ����չ���߼�����
}

// ���� Hole �ɼ���
void AHole::SetVisibility(bool bVisible)
{
	bIsVisible = bVisible;
	UpdateVisibility();
}

// �ڲ����� Hole �ɼ���
void AHole::UpdateVisibility()
{
	SetActorHiddenInGame(!bIsVisible);
}

// ���� Hole ��״̬
void AHole::SetHoleState(const FString& State)
{
	HoleState = State;
	UE_LOG(LogTemp, Log, TEXT("Hole state set to: %s"), *State);

	// ����״̬�޸� Hole ��ۻ���
	if (HoleState == "Empty")
	{
		// �������
		SetVisibility(true);
	}
	else if (HoleState == "Filled")
	{
		SetVisibility(false);
	}
}

// ������ʱ��
void AHole::StartTimer(float Duration)
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AHole::OnTimerCompleted, Duration, false);
}

// ��ʱ�����ʱ�ص�
void AHole::OnTimerCompleted()
{
	SetHoleState("Default");
}


