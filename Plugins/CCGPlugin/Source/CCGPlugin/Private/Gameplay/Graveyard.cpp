// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Graveyard.h"


// Sets default values
AGraveyard::AGraveyard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

