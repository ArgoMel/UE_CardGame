#pragma once
#include "../../CardTest.h"
#include "GameFramework/Actor.h"
#include "Dice.generated.h"

UCLASS()
class CARDTEST_API ADice : public AActor
{
	GENERATED_BODY()
public:	
	ADice();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Dice")
	TObjectPtr<UStaticMeshComponent> mDiceMesh;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Dice")
	void Rolling();
	void Rolling_Implementation();
};
