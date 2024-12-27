// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/BoardPlayer.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ABoardPlayer::ABoardPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoardPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerHealth);
	
	DOREPLIFETIME(ThisClass,mPlayerIndex);
	DOREPLIFETIME(ThisClass,mPlayerName);
}

void ABoardPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ABoardPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoardPlayer::OnRep_Health()
{
}

void ABoardPlayer::Server_UpdateHealth_Implementation(int32 Health)
{
}
