#include "Fruit.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"

// ���캯��
AFruit::AFruit()
{
	// ���� Tick
	PrimaryActorTick.bCanEverTick = true;

	// ��ʼ�� FruitBox
	FruitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FruitBox"));
	FruitBox->SetupAttachment(RootComponent);

	// ��ʼ�� FruitSprite
	FruitSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FruitSprite"));
	FruitSprite->SetupAttachment(FruitBox);
	
	// Ĭ��������������
    JumpHeight = 150.0f; // �����߶�
    JumpDuration = 1.5f; // ����ʱ��
    bShouldMove = false; // Ĭ�ϲ��ƶ�
    MovementSpeed = 100.0f; // �ƶ��ٶ�
}

// ��Ϸ��ʼʱ����
void AFruit::BeginPlay()
{
	Super::BeginPlay();
	// ��¼��ʼλ��
    InitialLocation = GetActorLocation();

    // ���ó�ʼ�ɼ���
    SetFruitVisibility(true);
     
    // ��ʼ��ʱ����
    if (JumpCurve)
    {
        // ����ʱ����ص�
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("HandleJumpProgress"));
        JumpTimeline.AddInterpFloat(JumpCurve, ProgressFunction);

        // ����ʱ������ɻص�
        FOnTimelineEvent FinishFunction;
        FinishFunction.BindUFunction(this, FName("OnJumpFinished"));
        JumpTimeline.SetTimelineFinishedFunc(FinishFunction);

        // ����ʱ��������
        JumpTimeline.SetLooping(false);
        JumpTimeline.SetPlayRate(1.0f / JumpDuration);

        // ��ʼ����ʱ����
        JumpTimeline.PlayFromStart();
    }

    // ��ʼ����ײ���
    if (FruitBox)
    {
        FruitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        FruitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
        FruitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        FruitBox->OnComponentBeginOverlap.AddDynamic(this, &AFruit::OnFruitOverlap);
    }
}

// ÿ֡����
void AFruit::Tick(float DeltaTime)
{
    Super::Tick();

    // ����ʱ����
    if (JumpTimeline.IsPlaying())
    {
        JumpTimeline.TickTimeline(DeltaTime);
    }

    // ����������ֹͣ����
    if (!JumpTimeline.IsPlaying() && bShouldMove)
    {
        // ����������ִ�������߼�
        bShouldMove = false;  // ֹͣ�κκ����ƶ�
    }
}


// ���������Ľ���
void AFruit::HandleJumpProgress(float Value)
{
    // ���������ĵ�ǰλ��
    FVector NewLocation = InitialLocation;
    NewLocation.Z += Value * JumpHeight;

    // ����ˮ����λ��
    SetActorLocation(NewLocation);
}

// �������ʱ�Ļص�
void AFruit::OnJumpFinished()
{
    // ������ɺ�ֹͣ����
    bShouldMove = false;
}

//�л����� 
void AItems::SwitchType(const FString& Selection)
{
    if (DataTable && !Selection.IsEmpty())
    {
        // ���ݴ���� Selection �������ݱ��еĶ�Ӧ��
        FItemData* ItemData = DataTable->FindRow<FItemData>(FName(*Selection), TEXT(""));

        if (ItemData)
        {
            // ���µ�ǰ�е� RowName���Ա� UpdateSprite ���Ի���������¾���
            RowName = FName(*Selection);

            // ������Ʒ�ľ���
            UpdateSprite();
        }
    }
}

// ����ˮ���ƶ�����
void AFruit::MoveFruit(const FVector& Direction, float Speed)
{
	if (FruitBox)
	{
		FVector NewLocation = GetActorLocation() + (Direction * Speed);
		SetActorLocation(NewLocation);
	}
}
// ����ˮ���ɼ���
void AFruit::SetFruitVisibility(bool bVisible)
{
    if (FruitSprite)
    {
        FruitSprite->SetVisibility(bVisible, true);

        // ������ɼ����������ײ
        if (FruitBox)
        {
            FruitBox->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
        }
    }
}

// ���ˮ���Ƿ���Ч
bool AFruit::IsValidFruit() const
{
    return FruitSprite != nullptr && FruitBox != nullptr;
}

//�ջ��ʵ 
void AFruit::OnFruitCollected()
{
    // ����ˮ��������
    SetFruitVisibility(false);
    SetLifeSpan(1.0f); // �ӳ�����
}

//���� Sprite
void AItems::UpdateSprite()
{
    // ���������Ʋ������ݱ��е���
    FItemData* ItemData = DataTable->FindRow<FItemData>(RowName, TEXT(""));

    if (ItemData)
    {
        // ������Ʒ�ľ���
        PaperSprite->SetSprite(ItemData->Sprite);
    }
}

