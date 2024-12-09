#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dice.generated.h"

/*
	n��ü �ֻ��� ������ �뵵�� �������.
	�⺻������ 6�����޽��� ���ԵǾ� �ִ�.
	c++ �� ��ӹ޾Ƽ� ����� �������.

	��ӹ޾Ƽ� �ؾ��� ����
	1. �ֻ����� ������ ��ġ�� ������ ���Ѵ�.
	2. �ֻ����� �������� n��ü ������ ���ڸ� ����Ѵ�.
	3. �Ի��� ���ڸ� ��� ������ ���Ѵ�.
*/

UCLASS()
class CARDTEST_API ADice : public AActor
{
	GENERATED_BODY()
public:	
	ADice();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle mDiceTimer;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Dice")
	TObjectPtr<UStaticMeshComponent> mDiceMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Dice")
	FVector mDiceLoc;
	UPROPERTY(BlueprintReadWrite, Category = "Dice")
	int32 mRollingResult;

protected:
	//����÷��̿��� ���̺�(�ڽ��� �� ������Ʈ�� ���� ����)�� �޾ƿ���
	//������ �ش� ������ �ڽ��� �� �ȿ��� ������ x,y��ġ�� �޾ƿ��� �����°� ���Ѵ�
	UFUNCTION(BlueprintPure, Category = "Dice")
	virtual FVector GetRandomLocation();

	//���� ������ ����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Dice")
	virtual void CalculateDiceResult();

	//�������� ������ üũ�ϰ� ������ ������ �� ����Լ� ȣ��
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dice")
	void CheckStillRolling();
	void CheckStillRolling_Implementation();

public:
	//�ֻ��� ����� �����ϴ� ������ ���
	//��: ����÷��̿��� �޾Ƴ��� ���̺��� ����
	//���ڴ� ��� ������ ���� ����
	UFUNCTION(BlueprintCallable, Category = "Dice")
	virtual void SendRollingResult(int32 result=0);

	//�ֻ��� ������ �Լ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dice")
	void Rolling();
	void Rolling_Implementation();

	int32 GetRollingResult()
	{
		return mRollingResult;
	}
};
