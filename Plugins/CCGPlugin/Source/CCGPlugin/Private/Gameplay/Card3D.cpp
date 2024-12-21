// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Card3D.h"

#include "Net/UnrealNetwork.h"

ACard3D::ACard3D()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACard3D::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mCardDataTableName)
	DOREPLIFETIME(ThisClass,mCardSet)
	DOREPLIFETIME(ThisClass,mCardData)
	DOREPLIFETIME(ThisClass,mHealth)
	DOREPLIFETIME(ThisClass,mAttack)
	DOREPLIFETIME(ThisClass,bIsPlaced)
	DOREPLIFETIME(ThisClass,bInGraveyard)
	DOREPLIFETIME(ThisClass,mTurnPoint)
	DOREPLIFETIME(ThisClass,mType)
	DOREPLIFETIME(ThisClass,mPlacementOwner)
	DOREPLIFETIME(ThisClass,mOwningPlayerID)
}

void ACard3D::BeginPlay()
{
	Super::BeginPlay();
}

void ACard3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACard3D::OnRep_Attack()
{
}

void ACard3D::OnRep_Health()
{
}

void ACard3D::OnRep_TurnsAlive()
{
}

void ACard3D::OnRep_bCardActive()
{
}

void ACard3D::OnRep_OwningPlayerID()
{
}

void ACard3D::OnRep_ManaCostPlacement()
{
}

void ACard3D::OnRep_CardType()
{
}

void ACard3D::ForceMoveCardDirectlyToGraveyard_Implementation(AGraveyard* graveyard)
{
}

void ACard3D::MultiCast_UpdateCardVisual_Implementation(bool IsVisual)
{
}

void ACard3D::Server_SetCardData_Implementation(const FName& CardTableName, ECardSet CardRace, FCard CardStruct, bool SetCardDataFromStruct)
{
}

void ACard3D::MultiCast_Attacking_Implementation(AActor* board_player)
{
}

void ACard3D::MultiCast_UpdateVisualStats_Implementation()
{
}