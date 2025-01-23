// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Card3D.h"

#include "BFL/CardAbilityBFL.h"
#include "BFL/CCGBFL.h"
#include "BFL/DeckBFL.h"
#include "BFL/EffectBFL.h"
#include "BFL/MiscBFL.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameInstance/CCGameInstance.h"
#include "Gameplay/CardPlacement.h"
#include "Gameplay/Graveyard.h"
#include "GameState/CCGState.h"
#include "Interface/CardPreviewInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/CCGPlayerController.h"

ACard3D::ACard3D()
: bUseGate(false)
, mHomeDestination()
, mGoalDestination()
, mHomeRotation()
, mGoalRotation()
, mCardMovementState(E3DCardMovementState::Placing)
, mOwningPlayerID(0)
, mCardID(0)
, mInterpSpeed(15.f)
, mErrorTolerance(0.001f)
, bIsPlaced(false)
, bIsSelected(false)
, bIsAttacking(true)
, bInGraveyard(false)
, bCardActive(false)
, mCardSet(ECardSet::Empty)
, mTurnPoint(1)
, mTurnsAlive(0)
, bPreviewEnabled(false)
, bIsAbilityActive(true)
{
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant=true;
	bNetLoadOnClient=false;
	SetCanBeDamaged(false);
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	mBox=CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mBox->SetupAttachment(RootComponent);

	mNameText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameText"));
	mNameText->SetupAttachment(RootComponent);

	mCardTypeText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("CardTypeText"));
	mCardTypeText->SetupAttachment(RootComponent);

	mDescriptionText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("DescriptionText"));
	mDescriptionText->SetupAttachment(RootComponent);

	mCardBaseMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardBaseMesh"));
	mCardBaseMesh->SetupAttachment(RootComponent);

	mHeartMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeartMesh"));
	mHeartMesh->SetupAttachment(mCardBaseMesh);

	mHealthText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
	mHealthText->SetupAttachment(mHeartMesh);

	mManaGemMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ManaGemMesh"));
	mManaGemMesh->SetupAttachment(mCardBaseMesh);

	mManaCostText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("ManaCostText"));
	mManaCostText->SetupAttachment(mManaGemMesh);

	mAttackMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttackMesh"));
	mAttackMesh->SetupAttachment(mCardBaseMesh);

	mAttackText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("AttackText"));
	mAttackText->SetupAttachment(mAttackMesh);
}

void ACard3D::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mHomeDestination)
	
	DOREPLIFETIME(ThisClass,mPlacementOwner)
	DOREPLIFETIME(ThisClass,mOwningPlayerID)
	DOREPLIFETIME(ThisClass,bIsPlaced)
	DOREPLIFETIME(ThisClass,bInGraveyard)
	DOREPLIFETIME(ThisClass,bCardActive)
	
	DOREPLIFETIME(ThisClass,mCardData)
	DOREPLIFETIME(ThisClass,mCardDataTableName)
	DOREPLIFETIME(ThisClass,mCardSet)
	DOREPLIFETIME(ThisClass,mTurnPoint)
	DOREPLIFETIME(ThisClass,mTurnsAlive)
}

void ACard3D::BeginPlay()
{
	Super::BeginPlay();
	OnBeginCursorOver.AddDynamic(this,&ThisClass::OnCardBeginOverlap);
	OnEndCursorOver.AddDynamic(this,&ThisClass::OnCardEndOverlap);

	mGameInstance=Cast<UCCGameInstance>(GetGameInstance());
	mCardBaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	const UWorld* world = GetWorld();
	IF_RET_VOID(world);
	mGameState=Cast<ACCGState>(world->GetGameState());
}

void ACard3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseGate)
	{
		FTransform newTransform=GetActorTransform();
		bool bLocArrived;
		bool bRotArrived;
		newTransform.SetLocation(InterpSelfToLocation(DeltaTime,bLocArrived));
		newTransform.SetRotation(InterpSelfToRotation(DeltaTime,bRotArrived).Quaternion());
		if (bLocArrived&&bRotArrived)
		{
			ArrivedAtDestination();
		}
	}
}

void ACard3D::TakesDamage_Implementation(ACard3D* DealingCard, int32 DamageAttack, int32 DamageHealth)
{
	if (!HasAuthority())
	{
		return;
	}
	mDamageDealingCard=DealingCard;
	mCardData.Attack.Damage=FMath::Clamp(mCardData.Attack.Damage-DamageAttack,0,100);
	mCardData.Health.Health=FMath::Clamp(mCardData.Health.Health-DamageHealth,0,100);
	Multicast_DamageVisual();
	Multicast_SpawnEffect(ECardEffects::DecreasedHealth);
	if (!ValidateAbility())
	{
		return;
	}
	RunActiveAbility(EAbilityTrigger::TakesDamage);
	if (mCardData.Health.Health<=0)
	{
		bInGraveyard=true;
		MoveCardToGraveyard();
		Multicast_SpawnEffect(ECardEffects::OnDeath);
	}
	Multicast_UpdateVisualStats();
}

void ACard3D::ShowPreviewCard()
{
	IF_RET_VOID(mGameInstance);
	if (!mViewCardPreview)
	{
		mViewCardPreview=CreateWidget(mGameInstance->GetFirstLocalPlayerController(),mCardPreviewClass);
	}
	if (mViewCardPreview->Implements<UCardPreviewInterface>())
	{
		float scale=CCG_Default::CardPreviewScale;
		if (mGameInstance->IsMobilePlatform())
		{
			scale*=1.5f;
		}
		ICardPreviewInterface::Execute_InitiateCardPreview(mViewCardPreview,mCardDataTableName,mCardSet,this,scale);
	}
	mViewCardPreview->AddToViewport(CCG_ZOrder::CardPreview);
}

void ACard3D::OnActivePlayerTurn_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}
	mTurnPoint=1;
	bCardActive=true;
	if (ValidateAbility())
	{
		RunActiveAbility(EAbilityTrigger::StartOfTurn);
	}
}

void ACard3D::OnEndActivePlayerTurn_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}
	bCardActive=true;
	++mTurnsAlive;
	--mCardData.Health.LifeExpectancy;
	if (mCardData.Health.LifeExpectancy>0
		&&!bInGraveyard
		&&mTurnsAlive>=mCardData.Health.LifeExpectancy)
	{
		RunActiveAbility(EAbilityTrigger::DeathByLifeExpectancy);
		MoveCardToGraveyard();
	}
	else
	{
		RunActiveAbility(EAbilityTrigger::EndOfTurn);
	}
	Multicast_UpdateCardVisual(true);
}

void ACard3D::OnRep_Stat()
{
	Multicast_UpdateVisualStats();
	mCardTypeText->SetText(UEnum::GetDisplayValueAsText(mCardData.Type));
}

void ACard3D::OnRep_bCardActive()
{
	Multicast_UpdateCardVisual(bCardActive);
}

void ACard3D::OnCardBeginOverlap(AActor* TouchedActor)
{
	IF_RET_VOID(mGameInstance)
	if (mGameInstance->IsMobilePlatform())
	{
		return;
	}
	if (!mOwningPlayer)
	{
		mOwningPlayer=Cast<ACCGPlayerController>(mGameInstance->GetFirstLocalPlayerController());
	}
	IF_RET_VOID(mOwningPlayer);
	if (mOwningPlayer->GetCardPlayerState()!=ECardPlayerState::PendingAction
		||!bIsPlaced)
	{
		return;
	}
	bPreviewEnabled=true;
	FTimerHandle tempTH;
	GetWorldTimerManager().SetTimer(tempTH,this,&ThisClass::ShowPreviewCardTimer,0.5f);
}

void ACard3D::OnCardEndOverlap(AActor* TouchedActor)
{
	IF_RET_VOID(mGameInstance)
	if (mGameInstance->IsMobilePlatform()
		||!bIsPlaced)
	{
		return;
	}
	RemoveMouseOverPreview();
}

void ACard3D::ShowPreviewCardTimer()
{
	if (bIsSelected||bInGraveyard)
	{
		return;
	}
	if (bPreviewEnabled)
	{
		ShowPreviewCard();
	}
	else
	{
		RemoveMouseOverPreview();
	}
}

void ACard3D::StartDamageCardFlash()
{
	mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::Tint,FVector(FLinearColor::Red));
	mCardBaseMesh->SetScalarParameterValueOnMaterials(CCG_Mat_Param::Brightness,1.5f);

	FTimerDelegate tempTimerDel;
	FTimerHandle tempTH;
	tempTimerDel.BindUFunction(this, FName("ChangeTurnStateVisual"), ETurnState::UpdateCurrentState);
	GetWorld()->GetTimerManager().SetTimer(tempTH, tempTimerDel, 0.3f, false);
}

void ACard3D::StartAbilityCardFlash()
{
	mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::Tint,FVector(FLinearColor::Blue));
	mCardBaseMesh->SetScalarParameterValueOnMaterials(CCG_Mat_Param::Brightness,2.f);

	FTimerDelegate tempTimerDel;
	FTimerHandle tempTH;
	tempTimerDel.BindUFunction(this, FName("ChangeTurnStateVisual"), ETurnState::UpdateCurrentState);
	GetWorld()->GetTimerManager().SetTimer(tempTH, tempTimerDel, 0.3f, false);
}

void ACard3D::RemoveCardActor()
{
	Destroy();
}

void ACard3D::ChangeTurnStateVisual(ETurnState CardGameState) const
{
	switch (CardGameState)
	{
	case ETurnState::TurnActive:
		mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::Tint,FVector(FLinearColor::White));
		mCardBaseMesh->SetScalarParameterValueOnMaterials(CCG_Mat_Param::Brightness,1.3f);
		break;
	case ETurnState::TurnInactive:
		mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::Tint,FVector(FLinearColor(0.13f,0.13f,0.13f,1.f)));
		mCardBaseMesh->SetScalarParameterValueOnMaterials(CCG_Mat_Param::Brightness,1.3f);
		break;
	case ETurnState::UpdateCurrentState:
		mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::Tint,FVector(FLinearColor::White));
		mCardBaseMesh->SetScalarParameterValueOnMaterials(CCG_Mat_Param::Brightness,1.f);
		break;
	default: ;
	}
}

void ACard3D::RemoveMouseOverPreview()
{
	if (!bPreviewEnabled)
	{
		return;
	}
	bPreviewEnabled=false;
	if (mViewCardPreview)
	{
		mViewCardPreview->RemoveFromParent();
	}
}

bool ACard3D::ValidateAbility() const
{
	return !mCardData.Abilities.IsEmpty();
}

bool ACard3D::RunActiveAbility(EAbilityTrigger AbilityTrigger)
{
	if (!HasAuthority()
		||(!bIsAbilityActive&&!(mCardData.Type==ECardType::Spell||mCardData.Type==ECardType::Booster)))
	{
		return false;
	}
	
	int32 index=0;
	for (const auto& ability : mCardData.Abilities)
	{
		if (ability.Trigger==AbilityTrigger)
		{
			if (CheckAbilityUseState(EAbilityTrigger::None,ECardUseState::SingleUse_SendToGraveyardBeforeAbility,index,false))
			{
				MoveCardToGraveyard();
			}
			Server_ActivateAbility(index);
		}
		++index;
	}
	return true;
}

bool ACard3D::CheckAbilityUseState(EAbilityTrigger CheckAbilityTrigger, ECardUseState CheckCardUseState,	int32 AbilityIndex, bool SearchAllAbilities)
{
	if (bInGraveyard)
	{
		return false;
	}
	if (SearchAllAbilities)
	{
		for (const auto& ability : mCardData.Abilities)
		{
			if (CheckAbilityTrigger==ability.Trigger
				&&ability.AfterUseState!=ECardUseState::RemainInPlay)
			{
				return true;
			}
		}
	}
	return mCardData.Abilities[AbilityIndex].AfterUseState==CheckCardUseState
		&&mCardData.Abilities[AbilityIndex].AfterUseState!=ECardUseState::RemainInPlay;
}

void ACard3D::SetTurnStateVisual(bool IsActive) const
{
	if (IsActive)
	{
		ChangeTurnStateVisual(ETurnState::TurnActive);
	}
	else
	{
		ChangeTurnStateVisual(ETurnState::TurnInactive);
	}
}

void ACard3D::SetCardData(FName CardTableName, ECardSet CardSet)
{
	mCardData=UDeckBFL::GetCardData(CardTableName,CardSet);
	mCardDataTableName=CardTableName;
	mCardSet=CardSet;
}

void ACard3D::SetCardVisuals()
{
	if (!UCCGBFL::CanExecuteCosmeticEvents(this))
	{
		return;
	}
	mCardBaseMesh->SetMaterial(1,mCardData.Visual.ImageMaterial);
	mCardBaseMesh->SetMaterial(2,mCardData.Visual.BackMaterial);
	mCardBaseMesh->SetMaterial(3,mCardData.Visual.FrameMaterial);
	mNameText->SetText(mCardData.Name);
	mCardTypeText->SetText(UEnum::GetDisplayValueAsText(mCardData.Type));
	GenerateDescription(mCardData.Description.Description);
	Multicast_UpdateVisualStats();
}

void ACard3D::MoveCardToGraveyard()
{
	if (!HasAuthority())
	{
		return;
	}
	GetWorldTimerManager().ClearTimer(mMoveToGraveyardTimer);
	bIsPlaced=false;
	mTurnPoint=0;
	bInGraveyard=true;
	mCardMovementState=E3DCardMovementState::Graveyard;
	if (ValidateAbility())
	{
		RunActiveAbility(EAbilityTrigger::SentToGraveyard);
	}
	bIsAbilityActive=false;
	IF_RET_VOID(mGameState);
	AGraveyard* graveyard=mGameState->GetGraveyardReference(mOwningPlayerID);
	if (mCardData.DeckSetting.AddToPlayerDeck
		&&graveyard)
	{
		graveyard->Server_AddToGraveyard(this);
		Multicast_MoveToGraveyard();
	}
	else
	{
		Multicast_SetCardHiddenState(true);
		Server_RemoveCardActor();
	}
	mGameState->RemoveCardOnBoard(this,mOwningPlayerID);
	if (mPlacementOwner)
	{
		mPlacementOwner->Server_RemoveCardFromPlacement(this);
	}
}

void ACard3D::PlayMovementAnimation(E3DCardMovementState CardMovementState, FVector ManualGoalDestination,	FRotator ManualGoalRotation, AActor* OR_GoalActor, float ZOffset, float ErrorTolerance, float InterpSpeed)
{
	mCardMovementState=CardMovementState;
	if (OR_GoalActor)
	{
		mGoalDestination=OR_GoalActor->GetActorLocation()+FVector(0.f,0.f,ZOffset);
		mGoalRotation=OR_GoalActor->GetActorRotation();
	}
	else
	{
		mGoalDestination=ManualGoalDestination;
		mGoalRotation=ManualGoalRotation;
	}
	mErrorTolerance=ErrorTolerance;
	mInterpSpeed=InterpSpeed;
	OpenGate();
}

void ACard3D::GenerateDescription(FString DescriptionText) const
{
	mDescriptionText->SetText(FText::GetEmpty());
	int32 curStringChar=0;
	for (int32 i=0;i<=DescriptionText.Len()/CCG_Default::MaxDescLineLength;++i)
	{
		FString text=mDescriptionText->Text.ToString();
		text.Append(DescriptionText.Mid(curStringChar,CCG_Default::MaxDescLineLength));
		text.Append("\n");
		mDescriptionText->SetText(FText::FromString(text));
		curStringChar+=CCG_Default::MaxDescLineLength;
	}
}

FRotator ACard3D::InterpSelfToRotation(float DeltaTime,bool& Arrived) const
{
	const FRotator curRot=GetActorRotation();
	if (mGoalRotation.Equals(curRot,mErrorTolerance/5.f))
	{
		return mGoalRotation;
	}
	return UKismetMathLibrary::RInterpTo(curRot,mGoalRotation,DeltaTime,mInterpSpeed);
}

FVector ACard3D::InterpSelfToLocation(float DeltaTime,bool& Arrived) const
{
	const FVector curLoc=GetActorLocation();
	if (mGoalDestination.Equals(curLoc,mErrorTolerance))
	{
		return mGoalDestination;
	}
	return UKismetMathLibrary::VInterpTo(curLoc,mGoalDestination,DeltaTime,mInterpSpeed);
}

bool ACard3D::RunCardAbility(int32 AbilityIndex)
{
	if (!HasAuthority())
	{
		return false;
	}
	switch (mCardData.Abilities[AbilityIndex].Type)
	{
	case EAbilityType::None:
		UMiscBFL::CreateScreenDisplayMessage(this,CCG_MSG::NoAbility,FLinearColor::Red,2.f);
		break;
	case EAbilityType::DrawCard:
		UCardAbilityBFL::DrawCard(this,AbilityIndex);
		break;
	case EAbilityType::IncreaseAttack:
		UCardAbilityBFL::IncreaseAttack(this,AbilityIndex);
		break;
	case EAbilityType::Clone:
		UCardAbilityBFL::CloneCard(this,AbilityIndex);
		break;
	case EAbilityType::IncreasePlayerHealth:
		UCardAbilityBFL::IncreasePlayerHealth(this,AbilityIndex);
		break;
	case EAbilityType::RetaliationDamage:
		UCardAbilityBFL::DealRetaliationDamage(this,AbilityIndex);
		break;
	case EAbilityType::DamageAllCardsOnBoard:
		UCardAbilityBFL::DamageAllCardsOnBoard(this,AbilityIndex);
		break;
	case EAbilityType::SpawnRandomCardFromDeck:
		UCardAbilityBFL::SpawnRandomCardFromDeck(this,AbilityIndex);
		break;
	case EAbilityType::GiveTurnPointsToAllActiveCards:
		UCardAbilityBFL::GiveTurnPointsToAllActiveCards(this,AbilityIndex);
		break;
	case EAbilityType::IncreaseTurnPoints:
		UCardAbilityBFL::IncreaseTurnPoints(this,AbilityIndex);
		break;
	case EAbilityType::DiscardCardsInHand:
		UCardAbilityBFL::DiscardRandomCardFromHand(this,AbilityIndex);
		break;
	case EAbilityType::PickupFirstCreatureInTheGraveyard_ToBoard:
		UCardAbilityBFL::PickupCardFromGraveyard(this,AbilityIndex,true,ECardType::Creature);
		break;
	case EAbilityType::PickupFirstCreatureInTheGraveyard_ToHand:
		UCardAbilityBFL::PickupCardFromGraveyard(this,AbilityIndex,false,ECardType::None);
		break;
	case EAbilityType::PossessOpponentsCard:
		UCardAbilityBFL::ChangePlayedCardOwner(this,AbilityIndex);
		break;
	case EAbilityType::SwapCardsInHand:
		UCardAbilityBFL::ChangeInHandCardOwner(this,AbilityIndex);
		break;
	default: ;
	}
	return true;
}

void ACard3D::SpawnEffect(ECardEffects CardEffect)
{
	if (!UCCGBFL::CanExecuteCosmeticEvents(this))
	{
		return;
	}
	USoundBase* sound;
	UParticleSystem* particle=UEffectBFL::GetCardEffectForState(CardEffect,mCardData,0,sound);
	UEffectBFL::SpawnParticleAndSoundEffect(this,particle,FVector::ZeroVector,this,FVector::OneVector,true,sound);
}

void ACard3D::OpenGate()
{
	bUseGate=true;
}

void ACard3D::CloseGate()
{
	bUseGate=false;
}

void ACard3D::ArrivedAtDestination()
{
	switch (mCardMovementState)
	{
	case E3DCardMovementState::Placing:
		break;
	case E3DCardMovementState::MoveToHomeDestination:
		if (!CheckAbilityUseState(EAbilityTrigger::OnDrop,ECardUseState::RemainInPlay,0,true))
		{
			mCardBaseMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		}
		CloseGate();
		SpawnEffect(ECardEffects::OnCardPlaced);
		if (ValidateAbility())
		{
			RunActiveAbility(EAbilityTrigger::OnDrop);
		}
		break;
	case E3DCardMovementState::MovingToDestination:
		break;
	case E3DCardMovementState::PlacedOnBoard:
		break;
	case E3DCardMovementState::Attacking:
		bIsAttacking=false;
		Multicast_SpawnEffect(ECardEffects::OnAttack);
		bCardActive=mTurnPoint>0;
		PlayMovementAnimation(E3DCardMovementState::MoveToHomeDestination,mGoalDestination,mGoalRotation,nullptr,50.f,5.f,12.f);
		break;
	case E3DCardMovementState::Selected:
		break;
	case E3DCardMovementState::Graveyard:
		CloseGate();
		if (mPlacementOwner)
		{
			mPlacementOwner->GetGraveyard()->Multicast_AddToGraveyardVisual();
		}
		break;
	default: ;
	}
}

void ACard3D::SetClientSideData(FName CardTableName, ECardSet CardRace)
{
	SetCardData(CardTableName,CardRace);
	SetCardVisuals();
}

void ACard3D::Selected(int32 SelectingPlayerID)
{
	if (bIsSelected)
	{
		return;
	}
	bIsSelected=true;
	SetActorScale3D(FVector(1.15f));
	if (SelectingPlayerID==mOwningPlayerID)
	{
		mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::OutlineColor,FVector(FLinearColor::Green));
	}
	else
	{
		mCardBaseMesh->SetVectorParameterValueOnMaterials(CCG_Mat_Param::OutlineColor,FVector(FLinearColor::Red));
	}
	RemoveMouseOverPreview();
}

void ACard3D::Deselected()
{
	if (!bIsSelected)
	{
		return;
	}
	bIsSelected=false;
	SetActorScale3D(FVector(1.f));
}

void ACard3D::EnableMobileCardPreview()
{
	if (bPreviewEnabled)
	{
		return;
	}
	IF_RET_VOID(mGameInstance);
	if (mGameInstance->IsMobilePlatform())
	{
		ShowPreviewCard();
	}
}

void ACard3D::DisableMobileCardPreview()
{
	if (!bPreviewEnabled)
	{
		return;
	}
	IF_RET_VOID(mGameInstance);
	if (mGameInstance->IsMobilePlatform())
	{
		RemoveMouseOverPreview();
	}
}

void ACard3D::Multicast_UpdateCardVisual_Implementation(bool IsVisual)
{
	SetTurnStateVisual(IsVisual);
}

void ACard3D::Multicast_UpdateVisualStats_Implementation()
{
	mAttackText->SetText(FText::AsNumber(mCardData.Attack.Damage));
	mHealthText->SetText(FText::AsNumber(mCardData.Health.Health));
	mManaCostText->SetText(FText::AsNumber(mCardData.PlacementSetting.ManaCost));
}

void ACard3D::Multicast_SpawnEffect_Implementation(ECardEffects CardEffect)
{
	SpawnEffect(CardEffect);
}

void ACard3D::Multicast_SetCardHiddenState_Implementation(bool IsHidden)
{
	SetActorHiddenInGame(IsHidden);
	mCardBaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACard3D::Multicast_PlaceCardOnBoard_Implementation(const FTransform& Destination)
{
	mHomeDestination=Destination.GetLocation();
	const FRotator playerRot=UMiscBFL::GetWorldRotationForPlayer(this,FRotator::ZeroRotator);
	if (!bIsPlaced)
	{
		bIsPlaced=true;
		SetActorRotation(playerRot);
		//그림자 쉐이더 활성화
	}
	mHomeRotation=playerRot+Destination.GetRotation().Rotator();
	PlayMovementAnimation(E3DCardMovementState::MoveToHomeDestination,mHomeDestination,mHomeRotation,nullptr,50.f,5.f,12.f);
}

void ACard3D::Multicast_Attacking_Implementation(AActor* Attacking)
{
	if (HasAuthority()
		&&!UCCGBFL::CanExecuteCosmeticEvents(this))
	{
		return;
	}
	bIsAttacking=true;
	mCardBaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayMovementAnimation(E3DCardMovementState::Attacking,FVector::ZeroVector,FRotator::ZeroRotator,Attacking,50.f,50.f,12.f);
}

void ACard3D::Multicast_MoveToGraveyard_Implementation()
{
	if (!UCCGBFL::CanExecuteCosmeticEvents(this))
	{
		return;
	}
	Deselected();
	SetTurnStateVisual(true);
	RemoveMouseOverPreview();
	mCardBaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//그림자 쉐이더 끄기
	IF_RET_VOID(mGameState);
	const AGraveyard* graveyard=mGameState->GetGraveyardReference(mOwningPlayerID);
	IF_RET_VOID(graveyard);
	PlayMovementAnimation(E3DCardMovementState::Graveyard,graveyard->GetCardInGraveyardLoc(0),graveyard->GetActorRotation(),nullptr,50.f,2.5f,12.f);
}

void ACard3D::Multicast_ForceMoveCardDirectlyToGraveyard_Implementation(AGraveyard* Graveyard)
{
	const FRotator playerRot=UMiscBFL::GetWorldRotationForPlayer(this,FRotator::ZeroRotator);
	SetActorRotation(playerRot);
	//그림자 쉐이더 설정
	IF_RET_VOID(Graveyard);
	PlayMovementAnimation(E3DCardMovementState::Graveyard,Graveyard->GetCardInGraveyardLoc(-1),Graveyard->GetActorRotation(),nullptr,50.f,2.5f,12.f);
}

void ACard3D::Multicast_SetCardVisuals_Implementation()
{
	SetCardVisuals();
}

void ACard3D::Multicast_DamageVisual_Implementation()
{
	ChangeTurnStateVisual(ETurnState::UpdateCurrentState);

	FTimerHandle tempTH;
	GetWorldTimerManager().SetTimer(tempTH,this,&ThisClass::StartDamageCardFlash,0.1f);
}

void ACard3D::Multicast_AbilityVisual_Implementation()
{
	ChangeTurnStateVisual(ETurnState::UpdateCurrentState);

	FTimerHandle tempTH;
	GetWorldTimerManager().SetTimer(tempTH,this,&ThisClass::StartAbilityCardFlash,0.1f);
}

void ACard3D::Server_SetCardData_Implementation(const FName& CardTableName, ECardSet CardRace)
{
	SetCardData(CardTableName,CardRace);
	Multicast_SetCardVisuals();
}

void ACard3D::Server_ActivateAbility_Implementation(int32 AbilityIndex)
{
	if (!HasAuthority())
	{
		return;
	}
	Multicast_AbilityVisual();
	Multicast_SpawnEffect(ECardEffects::OnAbilityUsed);
	if (RunCardAbility(AbilityIndex)
		&&CheckAbilityUseState(EAbilityTrigger::None,ECardUseState::SingleUse_SendToGraveyardAfterAbility,AbilityIndex,false)
		&&!mMoveToGraveyardTimer.IsValid())
	{
		GetWorldTimerManager().SetTimer(mMoveToGraveyardTimer,this,&ThisClass::MoveCardToGraveyard,1.f);
	}
}

void ACard3D::Server_RemoveCardActor_Implementation()
{
	FTimerHandle tempTH;
	GetWorld()->GetTimerManager().SetTimer(tempTH, this,&ACard3D::RemoveCardActor, 0.5f, false);
}