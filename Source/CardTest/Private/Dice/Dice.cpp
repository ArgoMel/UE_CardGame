#include "Dice/Dice.h"

ADice::ADice()
{
	PrimaryActorTick.bCanEverTick = false;

	mDiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiceMesh"));
	SetRootComponent(mDiceMesh);
}

void ADice::BeginPlay()
{
	Super::BeginPlay();
}

void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

