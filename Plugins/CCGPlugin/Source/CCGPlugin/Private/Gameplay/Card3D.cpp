// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Card3D.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameInstance/CCGameInstance.h"
#include "GameState/CCGState.h"
#include "Interface/CardPreviewInterface.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/CCGPlayerController.h"

ACard3D::ACard3D()
: bUseGate(false)
, mHomeDestination()
, mGoalDestination()
, mGoalRotation()
, mHomeRotation()
, mCardMovementState(E3DCardMovementState::Placing)
, mMaxDescCharacterLineLength(43)
, mOwningPlayerID(0)
, mCardID(0)
, mInterpSpeed(15.f)
, mErrorTolerance(0.001f)
, mCardPreviewScale(0.7f)
, bIsPlaced(false)
, bIsSelected(false)
, bIsAttacking(true)
, bInGraveyard(false)
, bEnableLifeExpectancy(false)
, bCardActive(false)
, mType(ECardType::Creature)
, mCardSet(ECardSet::Empty)
, mAttack(0)
, mHealth(0)
, mManaCostPlacement(0)
, mTurnPoint(1)
, mLifeExpectancy(0)
, mTurnsAlive(0)
, bCanAttackPlayer(false)
, bCanAttackCard(false)
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
		newTransform.SetLocation(InterpSelfToLocation(bLocArrived));
		newTransform.SetRotation(InterpSelfToRotation(bRotArrived).Quaternion());
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
	mAttack=FMath::Clamp(mAttack-DamageAttack,0,100);
	mHealth=FMath::Clamp(mHealth-DamageHealth,0,100);
	Multicast_DamageVisual();
	Multicast_SpawnEffect(ECardEffects::DecreasedHealth);
	if (!ValidateAbility())
	{
		return;
	}
	RunActiveAbility(EAbilityTrigger::TakesDamage);
	if (mHealth<=0)
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
		float scale=mCardPreviewScale;
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
	++mTurnPoint;
	if (bEnableLifeExpectancy
		&&!bInGraveyard
		&&mTurnsAlive>=mLifeExpectancy)
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
	if (HasAuthority())
	{
		if(bIsPlaced&&!bInGraveyard)
		{
			SaveCardStateToStruct();
		}
	}
}

void ACard3D::OnRep_TurnsAlive()
{
	if (HasAuthority())
	{
		if(bIsPlaced&&!bInGraveyard)
		{
			SaveCardStateToStruct();
		}
	}
}

void ACard3D::OnRep_bCardActive()
{
	Multicast_UpdateCardVisual(bCardActive);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ACard3D::OnRep_CardType()
{
	mCardTypeText->SetText(UEnum::GetDisplayValueAsText(mType));
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

void ACard3D::SaveCardStateToStruct()
{
	mCardData.Attack.Damage=mAttack;
	mCardData.Health.Health=mHealth;
	mCardData.Health.LifeExpectancy=mLifeExpectancy-mTurnsAlive;
	mCardData.PlacementSetting.ManaCost=mManaCostPlacement;
}

bool ACard3D::ValidateAbility() const
{
	return !mAbilityTypes.IsEmpty();
}

bool ACard3D::RunActiveAbility(EAbilityTrigger AbilityTrigger)
{
	if (!HasAuthority()
		||!mAbilityTriggers.Contains(AbilityTrigger)
		||(!bIsAbilityActive&&!(mType==ECardType::Spell||mType==ECardType::Booster)))
	{
		return false;
	}
	
	int32 index=0;
	for (const auto& ability : mAbilityTriggers)
	{
		if (ability==AbilityTrigger)
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

bool ACard3D::RunCardAbility(int32 AbilityIndex)
{
	return false;
}

void ACard3D::SpawnEffect(ECardEffects CardEffect)
{
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
}

void ACard3D::SetClientSideData(FName CardTableName, ECardSet CardRace)
{
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