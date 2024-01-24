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
	TObjectPtr<UStaticMeshComponent> mDiceMesh;
};
