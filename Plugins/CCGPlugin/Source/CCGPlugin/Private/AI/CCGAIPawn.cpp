// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CCGAIPawn.h"

#include "Net/UnrealNetwork.h"

ACCGAIPawn::ACCGAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACCGAIPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerID);
	DOREPLIFETIME(ThisClass,mCardGameAiID);
	
	DOREPLIFETIME(ThisClass,mAIStat);
}

void ACCGAIPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCGAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACCGAIPawn::OnRep_AIStat()
{
}

void ACCGAIPawn::IncreaseTurn()
{
	Execute_GetManaForTurn(this);
	++mPlayerTurn;
}

void ACCGAIPawn::UpdateAIState(int32 CardsInHand, int32 CardsInDeck)
{
}

void ACCGAIPawn::InitAttributes()
{
}

void ACCGAIPawn::Multicast_ForceUIUpdate_Implementation(int32 CardsInHand, int32 CardsInDeck)
{
}
