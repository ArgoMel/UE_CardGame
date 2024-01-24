#pragma once
#include "../../CardTest.h"
#include "GameFramework/Actor.h"
#include "Dice.generated.h"

/*
	n면체 주사위 굴리는 용도로 만들었다.
	기본적으로 6각형메쉬가 삽입되어 있다.
	c++ 로 상속받아서 쓰라고 만들었다.

	상속받아서 해야할 일은
	1. 주사위를 굴릴때 위치와 범위를 정한다.
	2. 주사위가 멈췄을때 n면체 맨위에 숫자를 계산한다.
	3. 게산한 숫자를 어디에 보낼지 정한다.
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
	//비긴플레이에서 테이블(박스나 원 컴포넌트를 갖는 액터)을 받아오고
	//굴릴때 해당 액터의 박스나 원 안에서 램덤한 x,y위치를 받아오고 오프셋과 더한다
	UFUNCTION(BlueprintPure, Category = "Dice")
	virtual FVector GetRandomLocation();

	//무슨 면인지 계산하는 함수
	UFUNCTION(BlueprintCallable, Category = "Dice")
	virtual void CalculateDiceResult();

	//굴러가능 중인지 체크하고 구르지 않으면 면 계산함수 호출
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dice")
	void CheckStillRolling();
	void CheckStillRolling_Implementation();

public:
	//주사위 결과를 전달하는 식으로 사용
	//예: 비긴플레이에서 받아놓은 테이블에다 전달
	//인자는 결과 주작을 위해 존재
	UFUNCTION(BlueprintCallable, Category = "Dice")
	virtual void SendRollingResult(int32 result=0);

	//주사위 굴리는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dice")
	void Rolling();
	void Rolling_Implementation();

	int32 GetRollingResult()
	{
		return mRollingResult;
	}
};
