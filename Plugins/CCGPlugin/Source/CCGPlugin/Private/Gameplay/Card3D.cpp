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
	DOREPLIFETIME(ThisClass,mHomeDestination)
	
	DOREPLIFETIME(ThisClass,mPlacementOwner)
	DOREPLIFETIME(ThisClass,mOwningPlayerID)
	DOREPLIFETIME(ThisClass,bIsPlaced)
	DOREPLIFETIME(ThisClass,bInGraveyard)
	DOREPLIFETIME(ThisClass,bEnableLifeExpectancy)
	DOREPLIFETIME(ThisClass,bCardActive)
	
	DOREPLIFETIME(ThisClass,mCardData)
	DOREPLIFETIME(ThisClass,mCardDataTableName)
	DOREPLIFETIME(ThisClass,mName)
	DOREPLIFETIME(ThisClass,mDescription)
	DOREPLIFETIME(ThisClass,mType)
	DOREPLIFETIME(ThisClass,mCardSet)
	DOREPLIFETIME(ThisClass,mAttack)
	DOREPLIFETIME(ThisClass,mHealth)
	DOREPLIFETIME(ThisClass,mManaCostPlacement)
	DOREPLIFETIME(ThisClass,mTurnPoint)
	DOREPLIFETIME(ThisClass,mLifeExpectancy)
	DOREPLIFETIME(ThisClass,mTurnsAlive)
	DOREPLIFETIME(ThisClass,bCanAttackPlayer)
	DOREPLIFETIME(ThisClass,bCanAttackCard)
	
	DOREPLIFETIME(ThisClass,mAbilityTriggers)
	DOREPLIFETIME(ThisClass,mAbilityTypes)
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

void ACard3D::SetClientSideData(FName CardTableName, ECardSet CardRace)
{
}

void ACard3D::EnableMobileCardPreview()
{
}

void ACard3D::DisableMobileCardPreview()
{
}

void ACard3D::Selected(int32 SelectingPlayerID)
{
}

void ACard3D::Deselected()
{
}

void ACard3D::Multicast_SetCardHiddenState_Implementation(bool IsHidden)
{
}

void ACard3D::Server_RemoveCardActor_Implementation()
{
}

void ACard3D::Multicast_ForceMoveCardDirectlyToGraveyard_Implementation(AGraveyard* Graveyard)
{
}

void ACard3D::Multicast_UpdateCardVisual_Implementation(bool IsVisual)
{
}

void ACard3D::Server_SetCardData_Implementation(const FName& CardTableName, ECardSet CardRace)
{
}

void ACard3D::Multicast_Attacking_Implementation(AActor* board_player)
{
}

void ACard3D::Multicast_UpdateVisualStats_Implementation()
{
}