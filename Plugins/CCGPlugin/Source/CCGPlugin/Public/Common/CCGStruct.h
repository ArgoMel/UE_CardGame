// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCGEnum.h"
#include "CCGStruct.generated.h"

class AGraveyard;
class ACardPlacement;
class ACard3D;

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Health;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 HealthMax;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Mana;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 ManaMax;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardsInHand;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardsInDeck;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 ActiveCard;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PlayerTurn;

	FPlayerStat()
	: Health(0)
	, HealthMax(100)
	, Mana(0)
	, ManaMax(0)
	, CardsInHand(0)
	, CardsInDeck(0)
	, ActiveCard(0)
	, PlayerTurn(0)
	{
	}

	/* 트루면 플레이 가능 상태 */
	FORCEINLINE bool CheckCardState() const
	{
		return CardsInDeck>0&&
			CardsInHand>0&&
				ActiveCard>0;	
	}
};

USTRUCT(BlueprintType)
struct FPlayerBoard
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ACard3D*> ActiveCards;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ACardPlacement*> CardPlacements;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<AGraveyard> Graveyards;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 TotalCardPlacementPositions;
	
	FPlayerBoard()
	: TotalCardPlacementPositions(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardGameOption
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FIntPoint Mana;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PlayerStartingHealth;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 MaxNumOfPlayers;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bSingleClientEnabled;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bSpectator;
	
	FCardGameOption()
	: Mana(FIntPoint(1,10))
	, PlayerStartingHealth(30)
	, MaxNumOfPlayers(2)
	, bSingleClientEnabled(false)
	, bSpectator(false)
	{
	}
};

#pragma region AIStruct
class AGraveyard;
class ACard3D;
class ACardPlacement;

USTRUCT(BlueprintType)
struct FAIPersonality: public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FName> PlayableDecks;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<EAIPersonalityFocus> PersonalityFocuses;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName AIName;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName AIPointAllocation;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EAIDifficulty Difficulty;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float PlaySpeed;

	FAIPersonality()
	: Difficulty()
	, PlaySpeed(1.f)
	{
		PlayableDecks.Add(TEXT("CloneDeck"));
		PersonalityFocuses.Add(EAIPersonalityFocus::DamageCards);
		AIName=TEXT("NONE");
		AIPointAllocation=TEXT("AI_Points_Default");
	}
};

USTRUCT(BlueprintType)
struct FCardInteraction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<AActor> TalkingCard;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<AActor> ReceivingActor;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EPlayType InteractionType;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 OwningControllerID;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Value;

	FCardInteraction()
	: InteractionType()
	, OwningControllerID(0)
	, Value(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FPlaySubStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FCardInteraction> CardPlayStructs;

	FPlaySubStruct()
	{
	}

	explicit FPlaySubStruct(TArray<FCardInteraction>& Array)
		:CardPlayStructs(Array)
	{
		
	}
};

USTRUCT(BlueprintType)
struct FPlayStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FPlaySubStruct> CardInteractions;

	FPlayStruct()
	{
	}

	explicit FPlayStruct(TArray<FCardInteraction>& Array)
	{
		CardInteractions.Add(FPlaySubStruct(Array));
	}
};

USTRUCT(BlueprintType)
struct FAIState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardOnBoardState;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardInHandState;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 HealthState;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Total;

	FAIState()
	: CardOnBoardState(0)
	, CardInHandState(0)
	, HealthState(0)
	, Total(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FCalculationPoint
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardRemovedFromPlay;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardDamaged;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PerDamagePointToPlayer;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PlayerRemovedFromPlay;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 OwningCardRemovedFromPlay;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 AttackOwnedCardWithValue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIncludeAIPointPerCard;

	FCalculationPoint()
	: CardRemovedFromPlay(5)
	, CardDamaged(2)
	, PerDamagePointToPlayer(2)
	, PlayerRemovedFromPlay(99)
	, OwningCardRemovedFromPlay(-3)
	, AttackOwnedCardWithValue(0)
	, bIncludeAIPointPerCard(true)
	{
	}
};

USTRUCT(BlueprintType)
struct FEvaluationPoint
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PerActiveCard;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PerCardInHand;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PerHealthPoint;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIncludeActiveCardValue;

	FEvaluationPoint()
	: PerActiveCard(0)
	, PerCardInHand(0)
	, PerHealthPoint(0)
	, bIncludeActiveCardValue(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FEvaluationState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float BoardStateCompValue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float InHandStateCompValue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float HealthCompValue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Total;

	FEvaluationState()
	: BoardStateCompValue(0.f)
	, InHandStateCompValue(0.f)
	, HealthCompValue(0.f)
	, Total(0.f)
	{
	}
};

USTRUCT(BlueprintType)
struct FPointAllocation: public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FEvaluationPoint Evaluation;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCalculationPoint Calculation;

	FPointAllocation()
	{
	}
};
#pragma endregion

#pragma region CardLayoutStruct
USTRUCT(BlueprintType)
struct FCardLayout: public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector2D TransformPivot;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EPositioningProfile PositioningProfile;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 MaxCardsInHand;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float RadialOffset;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float SpacingMin;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float SpacingMax;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float ScreenOffsetX;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float ScreenOffsetY;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float RotationScaleFactor;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Scale;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float Offset;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool DynamicSpacing;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool FlyInDirection;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool RotateCardsToOffset;

	FCardLayout()
	: TransformPivot(0.5f,1.f)
	, PositioningProfile(EPositioningProfile::Fanned)
	, MaxCardsInHand(7)
	, RadialOffset(-0.05f)
	, SpacingMin(180.f)
	, SpacingMax(440.f)
	, ScreenOffsetX(50.f)
	, ScreenOffsetY(18.f)
	, RotationScaleFactor(0.15f)
	, Scale(0.8f)
	, Offset(0.f)
	, DynamicSpacing(false)
	, FlyInDirection(true)
	, RotateCardsToOffset(true)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardLayoutCalculate
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector2D CardSpacing;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector2D ScreenOffset;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardLoopIndex; 
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardsInHand;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 MaxCardInHand;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	double CardOffset;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	double RotationDistanceScaleFactor;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	double RadiusOffset;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	double CardScale;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool FlyInDirection;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool RotateCardsToOffset;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool DynamicSpacing;

	FCardLayoutCalculate()
		: CardSpacing()
		, ScreenOffset()
		, CardLoopIndex(0)
		, CardsInHand(0)
		, MaxCardInHand(0)
		, CardOffset(0)
		, RotationDistanceScaleFactor(0)
		, RadiusOffset(0)
		, CardScale(0)
		, FlyInDirection(false)
		, RotateCardsToOffset(false)
		, DynamicSpacing(false)
	{
	}
};
#pragma endregion

#pragma region CardStruct
USTRUCT(BlueprintType)
struct FCardAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<USoundCue> SoundCue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EAbilityType Type;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EAbilityTrigger Trigger;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardUseState AfterUseState;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardPlayers AffectingPlayer;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 AbilityInt;

	FCardAbility()
	: Type()
	, Trigger()
	, AfterUseState()
	, AffectingPlayer()
	, AbilityInt(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardAttack
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<USoundCue> SoundCue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardDamageType DamageType;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Damage;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 ManaCost;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool CanAttackPlayer;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool CanAttackCards;

	FCardAttack()
	: DamageType(ECardDamageType::Basic)
	, Damage(1)
	, ManaCost(0)
	, CanAttackPlayer(true)
	, CanAttackCards(true)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector2D ScreenSize;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 MaxNumInDeck;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Weight;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool Unlocked;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool AddToPlayerDeck;

	FCardSetting()
	: ScreenSize()
	, MaxNumInDeck(3)
	, Weight(0)
	, Unlocked(true)
	, AddToPlayerDeck(true)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardSystemData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<EAIPersonalityFocus> CardPurposes;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ECardPlayConditions> PlayConditions;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<EInteractionConditions> InteractionConditions;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 CardValue;

	FCardSystemData()
	: CardValue(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardDescription
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString Description;

	FCardDescription()
	{
		Description=TEXT("(Description Top)");
	}
};

USTRUCT(BlueprintType)
struct FCardHealth
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UParticleSystem> DeathParticle;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<USoundCue> DeathSoundCue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Health;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 LifeExpectancy;

	FCardHealth()
	: Health(1)
	, LifeExpectancy(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardPlacementData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<USoundCue> SoundCue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardPlayers Owner;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 ManaCost;

	FCardPlacementData()
	: Owner()
	, ManaCost(1)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardVisual
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UMaterialInterface> FrameMaterial;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UMaterialInterface> ImageMaterial;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TObjectPtr<UMaterialInterface> BackMaterial;

	FCardVisual()
	{
	}
};

USTRUCT(BlueprintType)
struct FCard : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FCardAbility> Abilities;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardVisual Visual;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardAttack Attack;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardHealth Health;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardDescription Description;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardPlacementData PlacementSetting;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardSetting DeckSetting;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCardSystemData CardSystemData;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText Name;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardClass Class;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardType Type;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ECardRarity Rarity;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool Special;

	FCard()
	: Class(ECardClass::Warrior)
	, Type(ECardType::Creature)
	, Rarity(ECardRarity::Common)
	, Special(false)
	{
		Name=FText::FromString(TEXT("[Name]"));
		DeckSetting.ScreenSize=FVector2D(512.f,768.f);
	}
};
#pragma endregion

#pragma region GameStruct
USTRUCT(BlueprintType)
struct FBattleHistory
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCard TalkingCard;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FCard ReceivingCard;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EPlayType InteractionType;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 OwningControllerID;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 ReceivingPlayerID;

	FBattleHistory()
	: InteractionType()
	, OwningControllerID(0)
	, ReceivingPlayerID(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FPlayerStateSnapshot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FCard> CardArray;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FName> CardsInHand;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FName> Deck;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<int32> CardIDArray;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 PlayerID;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Health;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Mana;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Turn;

	FPlayerStateSnapshot()
	: PlayerID(0)
	, Health(0)
	, Mana(0)
	, Turn(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FGameStateSnapshot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FPlayerStateSnapshot> PlayerSnapshots;

	FGameStateSnapshot()
	{
	}
};

USTRUCT(BlueprintType)
struct FPrebuildDeckSub
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName CardTableName;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int32 Quantity;

	FPrebuildDeckSub()
	: Quantity(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FPrebuildDeck: public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FPrebuildDeckSub> CardsInDeck;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText DeckName;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString DeckDescription;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bEditable;

	FPrebuildDeck()
	: bEditable(false)
	{
	}
};
#pragma endregion