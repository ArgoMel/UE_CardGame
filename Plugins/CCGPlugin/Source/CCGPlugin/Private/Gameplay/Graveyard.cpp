// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Graveyard.h"

#include "CCGPlugin.h"
#include "BFL/CardBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/DeckBFL.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Gameplay/ActorComponent/AutoRotateToPlayer.h"
#include "Interface/ControllerInterface.h"
#include "Net/UnrealNetwork.h"

AGraveyard::AGraveyard()
: mPlayerIndex(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;
	SetNetUpdateFrequency(50.f);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mCollisionBox=CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	mCollisionBox->SetupAttachment(RootComponent);

	mGraveyardText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("GraveyardText"));
	mGraveyardText->SetupAttachment(RootComponent);

	mGraveyardMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GraveyardMesh"));
	mGraveyardMesh->SetupAttachment(RootComponent);
	mGraveyardMesh->SetReceivesDecals(false);

	mOutline=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outline"));
	mOutline->SetupAttachment(RootComponent);
	mOutline->SetReceivesDecals(false);

	mPlayerRotation=CreateDefaultSubobject<UAutoRotateToPlayer>(TEXT("PlayerRotation"));
}

void AGraveyard::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mActorsInGraveyard);
	DOREPLIFETIME(ThisClass,mGraveyardList);
}

void AGraveyard::BeginPlay()
{
	Super::BeginPlay();
	mGraveyardMesh->SetHiddenInGame(true);
}

void AGraveyard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGraveyard::ScaleCardGraveyardStaticMesh()
{
	mGraveyardMesh->SetHiddenInGame(mGraveyardList.IsEmpty());
	const FVector graveLoc(0.f,0.f,mGraveyardList.Num()*1.5f-66.f);
	SetActorRelativeLocation(graveLoc);
}

void AGraveyard::HideUnseenCardsInGraveyard()
{
	if (mActorsInGraveyard.Num()<2)
	{
		return;
	}
	for (auto& card3D : mActorsInGraveyard)
	{
		if (card3D
			&&card3D!=mActorsInGraveyard.Last())
		{
			card3D->Server_RemoveCardActor();
			mActorsInGraveyard.Remove(card3D);
		}
	}
}

FVector AGraveyard::GetCardInGraveyardLoc(int32 Add) const
{
	FVector newLoc=GetActorLocation();
	newLoc.Z+=(mGraveyardList.Num()+Add)*1.5f+10.f;
	return newLoc;
}

void AGraveyard::RecreateTopGraveyardCardActor()
{
	if (!HasAuthority()
		||mGraveyardList.IsEmpty()
		||!mActorsInGraveyard.IsEmpty())
	{
		return;
	}
	const AController* controller=UControllerBFL::GetControllerReferenceFromID(this,mPlayerIndex);
	IF_RET_VOID(controller);
	const FVector graveyardLoc=GetCardInGraveyardLoc(1);
	IF_RET_VOID(controller->Implements<UControllerInterface>());
	ACard3D* card3D=IControllerInterface::Execute_CreatePlayableCard(this,FTransform(graveyardLoc));
	card3D=UCardBFL::SetupCard(card3D,mPlayerIndex,mGraveyardList.Last(),ECardSet::Empty);
	mActorsInGraveyard.AddUnique(card3D);
	card3D->Multicast_ForceMoveCardDirectlyToGraveyard(this);
}

void AGraveyard::RemoveCardFromGraveyard(FName CardName, int32 Index)
{
	if (mGraveyardList.IsEmpty()
		||CardName!=mGraveyardList[Index])
	{
		return;
	}
	if (!mActorsInGraveyard.IsEmpty())
	{
		mActorsInGraveyard[0]->Multicast_SetCardHiddenState(true);
		mActorsInGraveyard[0]->Server_RemoveCardActor();
		mActorsInGraveyard.RemoveAt(0);
	}
	mGraveyardList.RemoveAt(Index);
}

void AGraveyard::Multicast_AddToGraveyardVisual_Implementation()
{
	ScaleCardGraveyardStaticMesh();
}

void AGraveyard::Server_AddToGraveyard_Implementation(ACard3D* Card, FName Name)
{
	if (Card)
	{
		mActorsInGraveyard.Add(Card);
		mGraveyardList.Add(Card->GetCardDataTableName());
	}
	else
	{
		const FCard card=UDeckBFL::GetCardData(Name,ECardSet::Empty);
		mGraveyardList.Add(Name);
	}
	HideUnseenCardsInGraveyard();
}

void AGraveyard::Server_RemoveCardFromGraveyard_Implementation(FName Card, int32 Index)
{
	RemoveCardFromGraveyard(Card, Index);
	RecreateTopGraveyardCardActor();
}
