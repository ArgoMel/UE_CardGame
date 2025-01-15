// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/ActorComponent/AutoRotateToPlayer.h"

#include "BFL/MiscBFL.h"

UAutoRotateToPlayer::UAutoRotateToPlayer()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval=0.2f;
}

void UAutoRotateToPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void UAutoRotateToPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FRotator playerRot = UMiscBFL::GetWorldRotationForPlayer(this,FRotator::ZeroRotator);
	GetOwner()->SetActorRotation(playerRot);
}

