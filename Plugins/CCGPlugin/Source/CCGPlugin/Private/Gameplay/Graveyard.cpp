// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Graveyard.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AGraveyard::AGraveyard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGraveyard::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mGraveyardList);
}

// Called when the game starts or when spawned
void AGraveyard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGraveyard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AGraveyard::GetCardInGraveyardLoc(int32 Add)
{
	return FVector::ZeroVector;
}

void AGraveyard::Server_AddToGraveyard_Implementation(ACard3D* Card, FName Name)
{
}

void AGraveyard::Server_RemoveCardFromGraveyard_Implementation(FName Card, int32 Index)
{
}
