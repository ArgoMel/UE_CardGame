// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Card3D.h"


// Sets default values
ACard3D::ACard3D()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACard3D::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACard3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

