// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TargetDragSelection.h"

ATargetDragSelection::ATargetDragSelection()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATargetDragSelection::BeginPlay()
{
	Super::BeginPlay();
}

void ATargetDragSelection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATargetDragSelection::SetSelectionProperties(FVector Loc, bool ValidTarget)
{
}

