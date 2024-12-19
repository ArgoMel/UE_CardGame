// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/CardPlacement.h"

#include "Net/UnrealNetwork.h"


// Sets default values
ACardPlacement::ACardPlacement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACardPlacement::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mCards);
	
	DOREPLIFETIME(ThisClass,mGraveyard);
	DOREPLIFETIME(ThisClass,mPlayerIndex);
	DOREPLIFETIME(ThisClass,bMultiPlacement);
	DOREPLIFETIME(ThisClass,bIsFull);
}

// Called when the game starts or when spawned
void ACardPlacement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardPlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACardPlacement::Server_AddCardToBoardPlacement_Implementation(ACard3D* CardToAdd)
{
}

