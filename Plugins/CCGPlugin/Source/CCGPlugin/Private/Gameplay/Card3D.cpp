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
	DOREPLIFETIME(ThisClass,mAttack)
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