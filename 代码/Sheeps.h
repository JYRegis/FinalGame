#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "GameFramework/Actor.h"
#include "Sheeeps.generated.h"

UCLASS()
class LEARN_2D_01_API ASheeeps : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// ���캯��
	ASheeeps();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// �����ƶ�����
	UFUNCTION(BlueprintCallable, Category = "Sheeeps")
	void StartMovement();

	// ��ʱ���ص�����
	void TimerCallback();

	// ����Ŀ��λ��
	UFUNCTION(BlueprintCallable, Category = "Sheeeps")
	void SetTargetPosition(FVector NewTargetPosition);

private:
	// ����Ŀ��λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sheeeps", meta = (AllowPrivateAccess = "true"))
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
#include "Sheeps.generated.h"

UCLASS()
class LEARN_2D_01_API ASheeps : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	// ���캯��
	ASheeps();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

public:
	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// �����ƶ�����
	UFUNCTION(BlueprintCallable, Category = "Sheeps")
	void StartMovement();

	// ��ʱ���ص�����
	void TimerCallback();

	// ����Ŀ��λ��
	UFUNCTION(BlueprintCallable, Category = "Sheeps")
	void SetTargetPosition(FVector NewTargetPosition);

private:
	// ����Ŀ��λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sheeps", meta = (AllowPrivateAccess = "true"))
	FVector TargetPosition;

	// ��ʱ�����
	FTimerHandle MovementTimerHandle;

	// AI�ƶ�����
	void MoveToTarget();
};
