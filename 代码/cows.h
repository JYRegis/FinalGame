#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "GameFramework/Actor.h"
#include "Cows.generated.h"

UCLASS()
class LEARN_2D_01_API ACows : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// ���캯��
	ACows();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// �����ƶ�����
	UFUNCTION(BlueprintCallable, Category = "Cows")
	void StartMovement();

	// ��ʱ���ص�����
	void TimerCallback();

	// ����Ŀ��λ��
	UFUNCTION(BlueprintCallable, Category = "Cows")
	void SetTargetPosition(FVector NewTargetPosition);

private:
	// ����Ŀ��λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cows", meta = (AllowPrivateAccess = "true"))
	FVector TargetPosition;

	// ��ʱ�����
	FTimerHandle MovementTimerHandle;

	// AI�ƶ�����
	void MoveToTarget();

	// ����ʵ��ָ��
	UPROPERTY()
	class UAnimInstance* AnimInstance;

	// ���ɵ�BPʵ��ָ��
	UPROPERTY()
	AActor* SpawnedBPInstance;
};

