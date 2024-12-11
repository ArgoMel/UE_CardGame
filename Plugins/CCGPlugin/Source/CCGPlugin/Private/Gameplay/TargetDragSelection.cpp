// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TargetDragSelection.h"


// Sets default values
ATargetDragSelection::ATargetDragSelection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetDragSelection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetDragSelection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

