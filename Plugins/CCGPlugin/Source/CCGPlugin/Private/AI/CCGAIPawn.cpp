// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CCGAIPawn.h"

#include "CCGPlugin.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/BoardPlayer.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/CCGPlayerController.h"

ACCGAIPawn::ACCGAIPawn()
: mPlayerID(0)
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
	AActor::SetReplicateMovement(false);
	bAlwaysRelevant=true;
	SetNetUpdateFrequency(10000.f);
	NetPriority=1.f;
}

void ACCGAIPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerID);
	
	DOREPLIFETIME(ThisClass,mAIStat);
}

void ACCGAIPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ACCGAIPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mGameMode)
	{
		const UWorld* world = GetWorld();
		if (world)
		{
			mGameMode=Cast<ACCGMode>(world->GetAuthGameMode());
			if (mGameMode)
			{
				InitAttributes();
			}
		}
	}
}

void ACCGAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int32 ACCGAIPawn::IncreasePlayerHealth_Implementation(int32 IncreaseBy)
{
	if (!HasAuthority())
	{
		return mAIStat.Health;
	}
	mAIStat.Health=FMath::Clamp(mAIStat.Health+IncreaseBy,0,mAIStat.HealthMax);
	return mAIStat.Health;
}

int32 ACCGAIPawn::DecreasePlayerHealth_Implementation(int32 DecreaseBy)
{
	if (!HasAuthority())
	{
		return mAIStat.Health;
	}
	mAIStat.Health=FMath::Clamp(mAIStat.Health-DecreaseBy,0,mAIStat.HealthMax);
	IF_RET(mAIStat.Health,mGameMode);
	mGameMode->CheckPlayerState();
	return mAIStat.Health;
}

int32 ACCGAIPawn::IncreasePlayerMana_Implementation(int32 IncreaseBy)
{
	if (!HasAuthority())
	{
		return mAIStat.Mana;
	}
	mAIStat.Mana=FMath::Clamp(mAIStat.Mana+IncreaseBy,0,mAIStat.ManaMax);
	return mAIStat.Mana;
}

int32 ACCGAIPawn::DecreasePlayerMana_Implementation(int32 DecreaseBy)
{
	if (!HasAuthority())
	{
		return mAIStat.Mana;
	}
	mAIStat.Mana=FMath::Clamp(mAIStat.Mana-DecreaseBy,0,mAIStat.ManaMax);
	return mAIStat.Mana;
}

int32 ACCGAIPawn::GetManaForTurn_Implementation()
{
	IF_RET(mAIStat.Mana,mGameMode);
	mAIStat.Mana=mGameMode->GetTurnMana(mOwningController);
	mAIStat.ManaMax=mAIStat.Mana;
	return mAIStat.Mana;
}

void ACCGAIPawn::OnRep_AIStat()
{
	if (HasAuthority())
	{
		if (!mOwningController)
		{
			mOwningController=Cast<ACCGAIController>(GetController());
		}
		if (mOwningController
			&&mOwningController->GetBoardPlayer())
		{
			mOwningController->GetBoardPlayer()->Server_UpdateHealth(mAIStat.Health);
		}
	}
	UpdateUI();
}

void ACCGAIPawn::UpdateUI()
{
	if (!mPlayerController)
	{
		const UWorld* world = GetWorld();
		IF_RET_VOID(world);
		mPlayerController=Cast<ACCGPlayerController>(world->GetFirstPlayerController());
	}
	if (mPlayerController)
	{
		mPlayerController->Client_UpdateGameUI(mAIStat.CardsInHand==0);
	}
}

void ACCGAIPawn::IncreaseTurn()
{
	Execute_GetManaForTurn(this);
	++mAIStat.PlayerTurn;
}

void ACCGAIPawn::UpdateAIState(int32 CardsInHand, int32 CardsInDeck)
{
	mAIStat.CardsInHand=CardsInHand;
	mAIStat.CardsInDeck=CardsInDeck;
	UpdateUI();
}

void ACCGAIPawn::InitAttributes()
{
	if (HasAuthority())
	{
		mAIStat.Health=mGameMode->mCardGameOption.PlayerStartingHealth;
		mAIStat.HealthMax=mGameMode->mCardGameOption.PlayerStartingHealth;
	}
}

void ACCGAIPawn::Multicast_ForceUIUpdate_Implementation(int32 CardsInHand, int32 CardsInDeck)
{
	mAIStat.CardsInHand=CardsInHand;
	mAIStat.CardsInDeck=CardsInDeck;
}
