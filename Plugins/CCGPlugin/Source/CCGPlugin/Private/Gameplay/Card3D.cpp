// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Card3D.h"

#include "Net/UnrealNetwork.h"

ACard3D::ACard3D()
: bUseGate(false)
, mHomeDestination()
, mGoalDestination()
, mGoalRotation()
, mHomeRotation()
, mPlatform()
, mCardMovementState()
, mCardGameState()
, mMaxDescCharacterLineLength(0)
, mOwningPlayerID(0)
, mCardID(0)
, mDeltaTime(0)
, mInterpSpeed(0)
, mErrorTolerance(0)
, mCardPreviewScale(0)
, bIsPlaced(false)
, bIsSelected(false)
, bIsAttacking(false)
, bInGraveyard(false)
, bIsPlatformMobile(false)
, bEnableLifeExpectancy(false)
, bCardActive(false)
, mType()
, mCardSet()
, mAttack(0)
, mHealth(0)
, mManaCostPlacement(0)
, mTurnPoint(0)
, mLifeExpectancy(0)
, mTurnsAlive(0)
, bCanAttackPlayer(false)
, bCanAttackCard(false)
, bPreviewEnabled(false)
, bIsAbilityActive(false)
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
	OnActorBeginOverlap.AddDynamic(this,&ThisClass::OnCardBeginOverlap);
	OnActorEndOverlap.AddDynamic(this,&ThisClass::OnCardEndOverlap);
}

void ACard3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACard3D::TakesDamage_Implementation(ACard3D* DealingCard, int32 DamageAttack, int32 DamageHealth)
{
	ICardInteractionInterface::TakesDamage_Implementation(DealingCard, DamageAttack, DamageHealth);
}

void ACard3D::OnActivePlayerTurn_Implementation()
{
	ICardInteractionInterface::OnActivePlayerTurn_Implementation();
}

void ACard3D::OnEndActivePlayerTurn_Implementation()
{
	ICardInteractionInterface::OnEndActivePlayerTurn_Implementation();
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

void ACard3D::OnCardBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

void ACard3D::OnCardEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

void ACard3D::ChangeTurnStateVisual(ECardGameState CardGameState)
{
}

void ACard3D::RemoveMouseOverPreview()
{
}

void ACard3D::SaveCardStateToStruct()
{
}

bool ACard3D::ValidateAbility()
{
	return false;
}

bool ACard3D::RunActiveAbility(EAbilityTrigger AbilityTrigger)
{
	return false;
}

bool ACard3D::CheckAbilityUseState(EAbilityTrigger CheckAbilityTrigger, ECardUseState CheckCardUseState,	int32 AbilityIndex, bool SearchAllAbilities)
{
	return false;
}

void ACard3D::SetTurnStateVisual(bool IsActive)
{
}

void ACard3D::SetCardData(FName CardTableName, ECardSet CardSet)
{
}

void ACard3D::SetCardVisuals()
{
}

void ACard3D::MoveCardToGraveyard()
{
}

void ACard3D::PlayMovementAnimation(E3DCardMovementState CardMovementState, FVector ManualGoalDestination,	FRotator ManualGoalRotation, AActor* OR_GoalActor, float ZOffset, float ErrorTolerance, float InterpSpeed)
{
}

void ACard3D::GenerateDescription(FString DescriptionText)
{
}

FRotator ACard3D::InterpSelfToRotation(bool& Arrived)
{
	return FRotator();
}

FVector ACard3D::InterpSelfToLocation(bool& Arrived)
{
	return FVector();
}

void ACard3D::LogDisplayMessage(FString Message, FLinearColor SpecifiedColor)
{
}

bool ACard3D::RunCardAbility(int32 AbilityIndex)
{
	return false;
}

void ACard3D::SpawnEffect(ECardEffects CardEffect)
{
}

void ACard3D::OpenGate()
{
}

void ACard3D::CloseGate()
{
}

void ACard3D::ArrivedAtDestination()
{
}

void ACard3D::SetClientSideData(FName CardTableName, ECardSet CardRace)
{
}

void ACard3D::Selected(int32 SelectingPlayerID)
{
}

void ACard3D::Deselected()
{
}

void ACard3D::EnableMobileCardPreview()
{
}

void ACard3D::DisableMobileCardPreview()
{
}

void ACard3D::Multicast_UpdateCardVisual_Implementation(bool IsVisual)
{
}

void ACard3D::Multicast_UpdateVisualStats_Implementation()
{
}

void ACard3D::Multicast_SpawnEffect_Implementation(ECardEffects CardEffect)
{
}

void ACard3D::Multicast_SetCardHiddenState_Implementation(bool IsHidden)
{
}

void ACard3D::Multicast_PlaceCardOnBoard_Implementation(const FTransform& Destination)
{
}

void ACard3D::Multicast_Attacking_Implementation(AActor* board_player)
{
}

void ACard3D::Multicast_MoveToGraveyard_Implementation()
{
}

void ACard3D::Multicast_ForceMoveCardDirectlyToGraveyard_Implementation(AGraveyard* Graveyard)
{
}

void ACard3D::Multicast_SetCardVisuals_Implementation()
{
}

void ACard3D::Multicast_DamageVisual_Implementation()
{
}

void ACard3D::Multicast_AbilityVisual_Implementation()
{
}

void ACard3D::Server_SetCardData_Implementation(const FName& CardTableName, ECardSet CardRace)
{
}

void ACard3D::Server_ActivateAbility_Implementation(int32 AbilityIndex)
{
}

void ACard3D::Server_RemoveCardActor_Implementation()
{
}