#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hole.generated.h"

UCLASS()
class LEARN_2D_01_API AHole : public AActor
{
	GENERATED_BODY()

public:
	// ���캯��
	AHole();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

	// �Ƿ��Ѿ���ֲ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	bool bIsPlanted;

	// ����ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	int32 GrowthDuration;

	// �Ƿ���ջ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	bool bCanBeHarvested;

	// ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	FName RowName;

	// S_����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	FString SeedType;

	// S_���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
	FString CategoryType;
	
    // Actor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AActor> SpawnedActorClass;
    
    // ���������׶�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    int32 SeedGrowthStage;

    // ���ӵ�ǰSprite
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    UPaperSprite* CurrentSeedSprite;

    // ��ʾ����״̬
    UFUNCTION()
    void UpdateSeedSprite();
    
    // ������������
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void UpdateCropGrowth();

    // ��ʱ�����
    FTimerHandle GrowthTimerHandle;

    // ������������׶�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    int32 MaxGrowthStage;

    // ��������ʱ�䣨ÿ�׶Σ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole", meta = (AllowPrivateAccess = "true"))
    float GrowthDurationPerStage;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// ���� Hole �ɼ��Եĺ���
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void SetVisibility(bool bVisible);

	// �����Ʒ����
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void InteractWithItem(const FString& ItemType);

	// ���� Hole ��״̬
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void SetHoleState(const FString& State);

	// ���Ŷ�ʱ������
	UFUNCTION(BlueprintCallable, Category = "Hole")
	void StartTimer(float Duration);
	
    // �Զ����¼����ջ�
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void Harvest();
    
    // �Զ����¼�����ֲ
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void PlantSeed(const FString& SeedType, int32 GrowthStage);
	
	// �Զ����¼�����ˮ
    UFUNCTION(BlueprintCallable, Category = "Hole")
    void WaterCrop();
};


