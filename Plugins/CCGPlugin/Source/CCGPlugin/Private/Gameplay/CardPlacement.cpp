// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/CardPlacement.h"

#include "CCGPlugin.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Gameplay/Card3D.h"
#include "Net/UnrealNetwork.h"

ACardPlacement::ACardPlacement()
: mFillDirection(ECardDirection::Left)
, mPlayerIndex(0)
, mMaxCardsInPlacement(1)
, mPlacementWidth(1.f)
, mCardSpacing(330.f)
, mCardWidth(330.f)
, bScaleMultiCardPlacement(true)
, bIsFull(false)
, bDynamicSpacing(false)
, bCenterOut(true)
, bInvertPlacementDirection(false)
, mDemoCards(1)
, bDisplayDemoCards(false)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates=true;

	for (const ECardType cardType : TEnumRange<ECardType>())
	{
		if(cardType == ECardType::None)
		{
			continue;
		}
		mValidCardTypes.Add(cardType);
	}

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);
	
	mPlacementCollision=CreateDefaultSubobject<UBoxComponent>("PlacementCollision");
	mPlacementCollision->SetupAttachment(RootComponent);
	mPlacementCollision->SetRelativeLocation(FVector(0, 0, 4));
	mPlacementCollision->SetBoxExtent(FVector::OneVector);
	mPlacementCollision->SetEnableGravity(false);
	mPlacementCollision->CanCharacterStepUpOn=ECB_No;
	mPlacementCollision->SetCollisionProfileName(CCG_Col_Profile::CardPlacement);
}

void ACardPlacement::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mCards);
	
	DOREPLIFETIME(ThisClass,mGraveyard);
	DOREPLIFETIME(ThisClass,mPlayerIndex);
	DOREPLIFETIME(ThisClass,bIsFull);
}

void ACardPlacement::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	switch (mFillDirection)
	{
	case ECardDirection::Left:
		bCenterOut=false;
		bInvertPlacementDirection=false;
		break;
	case ECardDirection::CenterLeft:
		bCenterOut=true;
		bInvertPlacementDirection=false;
		break;
	case ECardDirection::CenterRight:
		bCenterOut=true;
		bInvertPlacementDirection=true;
		break;
	case ECardDirection::Right:
		bCenterOut=false;
		bInvertPlacementDirection=true;
		break;
	default: ;
	}

	mPlacementWidth=FMath::Clamp(FMath::GridSnap(mPlacementWidth,1.f),1.f,20.f);
	
	CreateVisualBoardTiles();

	float scaleY=160.f;
	if (bScaleMultiCardPlacement)
	{
		scaleY=mCardWidth*(FMath::Clamp(mPlacementWidth-1.f,1.f,mPlacementWidth)/2.f);
		if (mPlacementWidth>1.f)
		{
			scaleY+=160.f;
		}
	}
	mPlacementCollision->SetWorldScale3D(FVector(220.f,scaleY,5.f));

	for (const auto& demoMesh : mDemoMesh)
	{
		demoMesh->DestroyComponent();
	}
	mDemoMesh.Empty();

	if (bDisplayDemoCards)
	{
		for (int32 i=0;i<mDemoCards;++i)
		{
			UStaticMeshComponent* mesh = NewObject<UStaticMeshComponent>(this);
			mesh->RegisterComponent();
			mesh->SetStaticMesh(mCardMesh);
			mesh->SetHiddenInGame(true);
			FTransform placementTransform=GetCardInPlacementLocation(i,mMaxCardsInPlacement,mDemoCards);
			mesh->SetWorldTransform(placementTransform);
			mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mDemoMesh.Add(mesh);
		}
	}
}

void ACardPlacement::BeginPlay()
{
	Super::BeginPlay();
}

void ACardPlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACardPlacement::SetAllCardLocation()
{
	bIsFull=mCards.Num()>=mMaxCardsInPlacement;
	int32 index=0;
	for (const auto& card : mCards)
	{
		FTransform placementTransform=GetCardInPlacementLocation(index,mMaxCardsInPlacement,mCards.Num());
		card->Multicast_PlaceCardOnBoard(placementTransform);
		++index;
	}
}

FTransform ACardPlacement::GetCardInPlacementLocation(int32 CardIndex, int32 MaxCards, int32 CardsInPlacement) const
{
	FTransform placementTransform=FTransform::Identity;
	const FVector actorLoc=GetActorLocation();
	const FRotator actorRot=GetActorRotation();
	bool isOverlap;
	const float dynamicSpacingValue=CalculateDynamicSpacingLocation(CardIndex,MaxCards,CardsInPlacement,bCenterOut,isOverlap);
	
	FVector newLoc=actorLoc+FVector(0.f,0.f,10.f);
	if (mMaxCardsInPlacement>1)
	{
		
		const float temp1=bDynamicSpacing?dynamicSpacingValue:CalculateSpacingLocation(CardIndex,MaxCards,CardsInPlacement,mCardSpacing);
		const float temp2=bScaleMultiCardPlacement?temp1:0.f;
		const float tempY=bInvertPlacementDirection?-temp2:temp2;
		const FVector tempVec(0.f,tempY,17.f);
		newLoc=actorLoc+actorRot.RotateVector(tempVec);
	}
	placementTransform.SetLocation(newLoc);

	FRotator newRot=actorRot;
	float tempValue=2.f;
	const bool tempBool=bDynamicSpacing?isOverlap:mCardWidth<=mCardSpacing;
	if (tempBool
		&&CardsInPlacement>=1)
	{
		tempValue=0.f;
	}
	const float tempRoll=bInvertPlacementDirection?-tempValue:tempValue;
	newRot.Roll+=tempRoll;
	placementTransform.SetRotation(newRot.Quaternion());
	return placementTransform;
}

void ACardPlacement::RemoveCardFromPlacement(ACard3D* CardToClear)
{
	mCards.Remove(CardToClear);
}

void ACardPlacement::CreateVisualBoardTiles()
{
	TArray<AActor*> actors;
	GetAllChildActors(actors);
	for (int32 i=0;i<actors.Num();++i)
	{
		actors[i]->Destroy();
	}
	for (const auto& child : mChildActors)
	{
		child->DestroyComponent();
	}
	mChildActors.Empty();
	if (!bScaleMultiCardPlacement)
	{
		return;
	}
	
	const int32 lastIndex=FMath::FloorToInt32(mPlacementWidth);
	for (int32 i=0;i<lastIndex;++i)
	{
		UChildActorComponent* childActor = NewObject<UChildActorComponent>(this);
		childActor->RegisterComponent();
		if (lastIndex==1)
		{
			childActor->SetChildActorClass(mSingleTIle);
		}
		else if (i==0)
		{
			childActor->SetChildActorClass(mLeftEndTile);
		}
		else if (i==lastIndex-1)
		{
			childActor->SetChildActorClass(mRightEndTile);
		}
		else
		{
			childActor->SetChildActorClass(mCenterTile);
		}

		const float y=mCardWidth*(i-(lastIndex-1)/2.f);
		childActor->SetWorldLocation(GetActorLocation()+GetActorRightVector()*y+GetActorUpVector()*FVector(7.f));
		childActor->SetWorldRotation(GetActorRotation());
		mChildActors.Add(childActor);
	}
}

float ACardPlacement::CalculateDynamicSpacingLocation(int32 Index, int32 MaxCardsInPlacement, int32 CardInPlacement, bool CenterOut, bool& Overlap) const
{
	const float cardSpacing1=mCardSpacing*MaxCardsInPlacement;
	const float cardSpacing2=cardSpacing1/2.f-mCardSpacing/2.f;
	const float cardSpacing3=cardSpacing1-mCardSpacing;
	const float cardSpacing4=FMath::FloorToInt32(cardSpacing1/mCardWidth)-1.f;

	const float placement1=CardInPlacement-1.f;
	const float placement2=placement1>=1.f?placement1:1.f;
	const float placement3=FMath::Clamp(cardSpacing3/placement2,0.f,mCardWidth);
	
	const float center1=CenterOut?CardInPlacement:MaxCardsInPlacement;
	const float center2=CenterOut?Index-(center1/2.f-0.5f):Index;
	const float center3=center2*placement3;
	const float center4=CenterOut?center3:center3-cardSpacing2;

	Overlap=CardInPlacement-2.f<cardSpacing4;
	return center4;
}

float ACardPlacement::CalculateSpacingLocation(int32 Index, int32 MaxCardsInPlacement, int32 CardInPlacement, float Spacing) const
{
	const float placementValue=bCenterOut?CardInPlacement:MaxCardsInPlacement;
	return (Index-(placementValue/2.f-0.5f))*Spacing;
}

void ACardPlacement::FindAndSetAroundPlacement()
{
	const UWorld* world=GetWorld();
	FHitResult hitResult;
	const FVector leftLoc=mChildActors[0]->GetRelativeLocation()+GetActorRightVector()*-mCardSpacing;
	const FVector rightLoc=mChildActors.Last()->GetRelativeLocation()+GetActorRightVector()*mCardSpacing;
	FCollisionShape shape;
	shape.SetBox(FVector3f(100.f,100.f,150.f));
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	
	if (world->SweepSingleByChannel(hitResult,leftLoc,GetActorLocation(),FQuat::Identity,ECC_CardPlacement,shape,param))
	{
		//DrawDebugBox(world,(leftLoc+GetActorLocation())*0.5f,shape.GetExtent(),FColor::Red,false,2.f);
		//UE_LOG(LogTemp,Warning,TEXT("Sweep obj : %s"),*hitResult.GetActor()->GetFName().ToString());
		mPlacementLeft=Cast<ACardPlacement>(hitResult.GetActor());
	}
	else
	{
		mPlacementLeft=nullptr;
	}
	if (world->SweepSingleByChannel(hitResult,rightLoc,GetActorLocation(),FQuat::Identity,ECC_CardPlacement,shape,param))
	{
		//DrawDebugBox(world,(rightLoc+GetActorLocation())*0.5f,shape.GetExtent(),FColor::Red,false,2.f);
		//UE_LOG(LogTemp,Warning,TEXT("Sweep obj : %s"),*hitResult.GetActor()->GetFName().ToString());
		mPlacementRight=Cast<ACardPlacement>(hitResult.GetActor());
	}
	else
	{
		mPlacementRight=nullptr;
	}
}

void ACardPlacement::Server_RemoveCardFromPlacement_Implementation(ACard3D* CardToClear)
{
	RemoveCardFromPlacement(CardToClear);
	SetAllCardLocation();
}

void ACardPlacement::Server_AddCardToBoardPlacement_Implementation(ACard3D* CardToAdd)
{
	mCards.Add(CardToAdd);
	SetAllCardLocation();
}

