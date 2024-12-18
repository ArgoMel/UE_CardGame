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
	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	int32 HealthMax;
	UPROPERTY(EditAnywhere)
	int32 Mana;
	UPROPERTY(EditAnywhere)
	int32 ManaMax;
	UPROPERTY(EditAnywhere)
	int32 CardsInHand;
	UPROPERTY(EditAnywhere)
	int32 CardsInDeck;
	UPROPERTY(EditAnywhere)
	int32 ActiveCard;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TArray<ACard3D*> ActiveCards;
	UPROPERTY(EditAnywhere)
	TArray<ACardPlacement*> CardPlacements;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AGraveyard> Graveyards;
	UPROPERTY(EditAnywhere)
	int32 TotalCardPlacementPositions;
	
	FPlayerBoard()
	: TotalCardPlacementPositions(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FCardLayoutCalculate
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector2D CardSpacing;
	UPROPERTY(EditAnywhere)
	FVector2D ScreenOffset;
	UPROPERTY(EditAnywhere)
	int32 CardLoopIndex; 
	UPROPERTY(EditAnywhere)
	int32 CardsInHand;
	UPROPERTY(EditAnywhere)
	int32 MaxCardInHand;
	UPROPERTY(EditAnywhere)
	double CardOffset;
	UPROPERTY(EditAnywhere)
	double RotationDistanceScaleFactor;
	UPROPERTY(EditAnywhere)
	double RadiusOffset;
	UPROPERTY(EditAnywhere)
	double CardScale;
	UPROPERTY(EditAnywhere)
	bool FlyInDirection;
	UPROPERTY(EditAnywhere)
	bool RotateCardsToOffset;
	UPROPERTY(EditAnywhere)
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

#pragma region AIStruct
class AGraveyard;
class ACard3D;
class ACardPlacement;

USTRUCT(BlueprintType)
struct FAIPersonality
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FName> PlayableDecks;
	UPROPERTY(EditAnywhere)
	TArray<EAIPersonalityFocus> PersonalityFocuses;
	UPROPERTY(EditAnywhere)
	FName AIName;
	UPROPERTY(EditAnywhere)
	FName AIPointAllocation;
	UPROPERTY(EditAnywhere)
	EAIDifficulty Difficulty;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> TalkingCard;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> ReceivingActor;
	UPROPERTY(EditAnywhere)
	EPlayType InteractionType;
	UPROPERTY(EditAnywhere)
	int32 OwningControllerID;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	int32 CardOnBoardState;
	UPROPERTY(EditAnywhere)
	int32 CardInHandState;
	UPROPERTY(EditAnywhere)
	int32 HealthState;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	int32 CardRemovedFromPlay;
	UPROPERTY(EditAnywhere)
	int32 CardDamaged;
	UPROPERTY(EditAnywhere)
	int32 PerDamagePointToPlayer;
	UPROPERTY(EditAnywhere)
	int32 PlayerRemovedFromPlay;
	UPROPERTY(EditAnywhere)
	int32 OwningCardRemovedFromPlay;
	UPROPERTY(EditAnywhere)
	int32 AttackOwnedCardWithValue;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	int32 PerActiveCard;
	UPROPERTY(EditAnywhere)
	int32 PerCardInHand;
	UPROPERTY(EditAnywhere)
	int32 PerHealthPoint;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	float BoardStateCompValue;
	UPROPERTY(EditAnywhere)
	float InHandStateCompValue;
	UPROPERTY(EditAnywhere)
	float HealthCompValue;
	UPROPERTY(EditAnywhere)
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
struct FPointAllocation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FEvaluationPoint Evaluation;
	UPROPERTY(EditAnywhere)
	FCalculationPoint Calculation;

	FPointAllocation()
	{
	}
};
#pragma endregion

#pragma region CardLayoutStruct
USTRUCT(BlueprintType)
struct FCardLayout
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector2D TransformPivot;
	UPROPERTY(EditAnywhere)
	EPositioningProfile PositioningProfile;
	UPROPERTY(EditAnywhere)
	int32 MaxCardsInHand;
	UPROPERTY(EditAnywhere)
	float RadialOffset;
	UPROPERTY(EditAnywhere)
	float SpacingMin;
	UPROPERTY(EditAnywhere)
	float SpacingMax;
	UPROPERTY(EditAnywhere)
	float ScreenOffsetX;
	UPROPERTY(EditAnywhere)
	float ScreenOffsetY;
	UPROPERTY(EditAnywhere)
	float RotationScaleFactor;
	UPROPERTY(EditAnywhere)
	float Scale;
	UPROPERTY(EditAnywhere)
	float Offset;
	UPROPERTY(EditAnywhere)
	bool DynamicSpacing;
	UPROPERTY(EditAnywhere)
	bool FlyInDirection;
	UPROPERTY(EditAnywhere)
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
#pragma endregion

#pragma region CardStruct
USTRUCT(BlueprintType)
struct FCardAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> SoundCue;
	UPROPERTY(EditAnywhere)
	EAbilityType Type;
	UPROPERTY(EditAnywhere)
	EAbilityTrigger Trigger;
	UPROPERTY(EditAnywhere)
	ECardUseState AfterUseState;
	UPROPERTY(EditAnywhere)
	ECardPlayers AffectingPlayer;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> SoundCue;
	UPROPERTY(EditAnywhere)
	ECardDamageType DamageType;
	UPROPERTY(EditAnywhere)
	int32 Damage;
	UPROPERTY(EditAnywhere)
	int32 ManaCost;
	UPROPERTY(EditAnywhere)
	bool CanAttackPlayer;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	FVector2D ScreenSize;
	UPROPERTY(EditAnywhere)
	int32 MaxNumInDeck;
	UPROPERTY(EditAnywhere)
	int32 Weight;
	UPROPERTY(EditAnywhere)
	bool Unlocked;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TArray<EAIPersonalityFocus> CardPurposes;
	UPROPERTY(EditAnywhere)
	TArray<ECardPlayConditions> PlayConditions;
	UPROPERTY(EditAnywhere)
	TArray<EInteractionConditions> InteractionConditions;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> DeathParticle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> DeathSoundCue;
	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> SoundCue;
	UPROPERTY(EditAnywhere)
	ECardPlayers Owner;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> FrameTexture;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ImageTexture;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> BackTexture;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialExpression> FrameMaterial;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialExpression> ImageMaterial;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialExpression> BackMaterial;

	FCardVisual()
	{
	}
};

USTRUCT(BlueprintType)
struct FCard : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FCardAbility> Abilities;
	UPROPERTY(EditAnywhere)
	FCardVisual Visual;
	UPROPERTY(EditAnywhere)
	FCardAttack Attack;
	UPROPERTY(EditAnywhere)
	FCardHealth Health;
	UPROPERTY(EditAnywhere)
	FCardDescription Description;
	UPROPERTY(EditAnywhere)
	FCardPlacementData PlacementSetting;
	UPROPERTY(EditAnywhere)
	FCardSetting DeckSetting;
	UPROPERTY(EditAnywhere)
	FCardSystemData CardSystemData;
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	ECardClass Class;
	UPROPERTY(EditAnywhere)
	ECardType Type;
	UPROPERTY(EditAnywhere)
	ECardRarity Rarity;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	FCard TalkingCard;
	UPROPERTY(EditAnywhere)
	FCard ReceivingCard;
	UPROPERTY(EditAnywhere)
	EPlayType InteractionType;
	UPROPERTY(EditAnywhere)
	int32 OwningControllerID;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	TArray<FCard> CardArray;
	UPROPERTY(EditAnywhere)
	TArray<FName> CardsInHand;
	UPROPERTY(EditAnywhere)
	TArray<FName> Deck;
	UPROPERTY(EditAnywhere)
	TArray<int32> CardIDArray;
	UPROPERTY(EditAnywhere)
	int32 PlayerID;
	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	int32 Mana;
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditAnywhere)
	FName CardTableName;
	UPROPERTY(EditAnywhere)
	int32 Quantity;

	FPrebuildDeckSub()
	: Quantity(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FPrebuildDeck
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FPrebuildDeckSub> CardsInDeck;
	UPROPERTY(EditAnywhere)
	FText DeckName;
	UPROPERTY(EditAnywhere)
	FString DeckDescription;
	UPROPERTY(EditAnywhere)
	bool bEditable;

	FPrebuildDeck()
	: bEditable(false)
	{
	}
};
#pragma endregion