#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Fruit.generated.h"

/**
 * Fruit �࣬�̳��� AItems
 */
UCLASS()
class LEARN_2D_01_API AFruit : public AItems
{
	GENERATED_BODY()

public:
	// ���캯��
	AFruit();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

    virtual void Tick() override;
    
	// �����ײ�� Box ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FruitBox;

	// ��ʾˮ���� PaperSprite ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* FruitSprite;

	// �л�ˮ������
	UFUNCTION(BlueprintCallable, Category = "Fruit")
	void SwitchFruitType(const FString& FruitType);

private:
	// ����ˮ���ƶ�����
	void MoveFruit(const FVector& Direction, float Speed);

	// ����ˮ���ɼ���
	void SetFruitVisibility(bool bVisible);

	// ����ˮ��λ��
	void SetFruitLocation(const FVector& NewLocation);

	// ���ˮ����Ч��
	bool IsValidFruit() const;

	// ����ˮ������������ʹ��ʱ���ᣩ
	void HandleJumpProgress(float Value);
	void OnJumpFinished();

	// ��������������ʱ����
	FTimeline JumpTimeline;

	// ���������ĸ߶�
	UPROPERTY(EditAnywhere, Category = "Animation")
	float JumpHeight;

	// ���������ĳ���ʱ��
	UPROPERTY(EditAnywhere, Category = "Animation")
	float JumpDuration;

	// ��ǰˮ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString CurrentFruitType;

	// �����Ƿ���ɵı�־
	bool bShouldMove;

};



