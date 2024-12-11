// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CCGAIPawn.h"

#include "Net/UnrealNetwork.h"


// Sets default values
ACCGAIPawn::ACCGAIPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACCGAIPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerID);
	DOREPLIFETIME(ThisClass,mCardGameAiID);
	
	DOREPLIFETIME(ThisClass,mAIStat);
}

// Called when the game starts or when spawned
void ACCGAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCGAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACCGAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACCGAIPawn::OnRep_AIStat()
{
}

void ACCGAIPawn::UpdateAIState(int32 CardsInHand, int32 CardsInDeck)
{
}

void ACCGAIPawn::InitAttributes()
{
}