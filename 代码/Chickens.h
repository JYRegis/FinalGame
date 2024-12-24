#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "GameFramework/Actor.h"
#include "Chickens.generated.h"

UCLASS()
class LEARN_2D_01_API AChickens : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// ���캯��
	AChickens();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// �����ƶ�����
	UFUNCTION(BlueprintCallable, Category = "Chickens")
	void StartMovement();

	// ��ʱ���ص�����
	void TimerCallback();

	// ����Ŀ��λ��
	UFUNCTION(BlueprintCallable, Category = "Chickens")
	void SetTargetPosition(FVector NewTargetPosition);

private:
	// ����Ŀ��λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chickens", meta = (AllowPrivateAccess = "true"))
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
#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "GameFramework/Actor.h"
#include "Chickens.generated.h"

UCLASS()
class LEARN_2D_01_API AChickens : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// ���캯��
	AChickens();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// �����ƶ�����
	UFUNCTION(BlueprintCallable, Category = "Chickens")
	void StartMovement();

	// ��ʱ���ص�����
	void TimerCallback();

	// ����Ŀ��λ��
	UFUNCTION(BlueprintCallable, Category = "Chickens")
	void SetTargetPosition(FVector NewTargetPosition);

private:
	// ����Ŀ��λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chickens", meta = (AllowPrivateAccess = "true"))
	FVector TargetPosition;

	// ��ʱ�����
	FTimerHandle MovementTimerHandle;

	// AI�ƶ�����
	void MoveToTarget();
};
