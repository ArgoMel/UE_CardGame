#pragma once

#pragma region EffectEnum
UENUM(BlueprintType)
enum class EBoardPlayerEffects : uint8
{
	DecreasedHealth UMETA(DisplayName = "DecreasedHealth"),
	IncreasedHealth UMETA(DisplayName = "IncreasedHealth"),
	Death UMETA(DisplayName = "Death"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardEffects : uint8
{
	OnCardPlaced UMETA(DisplayName = "OnCardPlaced"),
	OnAttack UMETA(DisplayName = "OnAttack"),
	DecreasedHealth UMETA(DisplayName = "DecreasedHealth"),
	IncreasedHealth UMETA(DisplayName = "IncreasedHealth"),
	OnAbilityUsed UMETA(DisplayName = "OnAbilityUsed"),
	OnDeath UMETA(DisplayName = "OnDeath"),
	Max UMETA(Hidden)
};
#pragma endregion

#pragma region GameplayEnum
UENUM(BlueprintType)
enum class E3DCardMovementState : uint8
{
	Placing UMETA(DisplayName = "Placing"),
	MoveToHomeDestination UMETA(DisplayName = "MoveToHomeDestination"),
	MovingToDestination UMETA(DisplayName = "MovingToDestination"),
	PlacedOnBoard UMETA(DisplayName = "PlacedOnBoard"),
	Attacking UMETA(DisplayName = "Attacking"),
	Selected UMETA(DisplayName = "Selected"),
	Graveyard UMETA(DisplayName = "Graveyard"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAbilityTrigger : uint8
{
	None UMETA(DisplayName = "None"),
	OnDrop UMETA(DisplayName = "OnDrop"),
	StartOfTurn UMETA(DisplayName = "StartOfTurn"),
	EndOfTurn UMETA(DisplayName = "EndOfTurn"),
	TakesDamage UMETA(DisplayName = "TakesDamage"),
	SentToGraveyard UMETA(DisplayName = "SentToGraveyard"),
	DeathByLifeExpectancy UMETA(DisplayName = "DeathByLifeExpectancy"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None UMETA(DisplayName = "None"),
	DrawCard UMETA(DisplayName = "DrawCard(s)"),
	IncreaseAttack UMETA(DisplayName = "IncreaseAttack"),
	Clone UMETA(DisplayName = "Clone"),
	IncreasePlayerHealth UMETA(DisplayName = "IncreasePlayerHealth"),
	RetaliationDamage UMETA(DisplayName = "RetaliationDamage"),
	DamageAllCardsOnBoard UMETA(DisplayName = "DamageAllCardsOnBoard"),
	SpawnRandomCardFromDeck UMETA(DisplayName = "SpawnRandomCardFromDeck"),
	GiveTurnPointsToAllActiveCards UMETA(DisplayName = "GiveTurnPointsToAllActiveCards"),
	IncreaseTurnPoints UMETA(DisplayName = "IncreaseTurnPoints"),
	DiscardCardsInHand UMETA(DisplayName = "DiscardCardsInHand"),
	PickupFirstCreatureInTheGraveyard_ToBoard UMETA(DisplayName = "PickupFirstCreatureInTheGraveyard(ToBoard)"),
	PickupFirstCreatureInTheGraveyard_ToHand UMETA(DisplayName = "PickupFirstCreatureInTheGraveyard(ToHand)"),
	PossessOpponentsCard UMETA(DisplayName = "PossessOpponentsCard"),
	SwapCardsInHand UMETA(DisplayName = "SwapCardsInHand"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardClass : uint8
{
	None UMETA(DisplayName = "None"),
	Warrior UMETA(DisplayName = "Warrior"),
	Mage UMETA(DisplayName = "Mage"),
	Spy UMETA(DisplayName = "Spy"),
	Obelisk UMETA(DisplayName = "Obelisk"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardPlayConditions : uint8
{
	None UMETA(DisplayName = "None"),
	PlayAtStartOfTurn_AIOnly UMETA(DisplayName = "PlayAtStartOfTurn(AIOnly)"),
	PlayAtEndOfTurn_AIOnly UMETA(DisplayName = "PlayAtEndOfTurn(AIOnly)"),
	PlayWhenLowHealth_Self UMETA(DisplayName = "PlayWhenLowHealth(Self)"),
	PlayWhenHighHealth_Self UMETA(DisplayName = "PlayWhenHighHealth(Self)"),
	PlayWhenCreaturesAreOnTheBoard_Self UMETA(DisplayName = "PlayWhenHighHealth(Self)"),
	PlayWhenCreaturesAreOnTheBoard_Self_AIOnly UMETA(DisplayName = "PlayWhenHighHealth(Self)(AIOnly)"),
	PlayWhenCreaturesAreOnTheBoard_Opponent UMETA(DisplayName = "PlayWhenHighHealth(Opponent)"),
	PlayWhenCreaturesAreOnTheBoard_Opponent_AIOnly UMETA(DisplayName = "PlayWhenHighHealth(Opponent)(AIOnly)"),
	PlayWhenThereAreCardsInTheGraveyard UMETA(DisplayName = "PlayWhenThereAreCardsInTheGraveyard"),
	PlayWhenThereAreCardsInTheGraveyard_AIOnly UMETA(DisplayName = "PlayWhenThereAreCardsInTheGraveyard(AIOnly)"),
	PlayWhenLowPlayableCardInHand_AIOnly UMETA(DisplayName = "PlayWhenLowPlayableCardInHand(AIOnly)"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	UltraRare UMETA(DisplayName = "UltraRare"),
	Legendary UMETA(DisplayName = "Legendary"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardSet : uint8
{
	Empty UMETA(DisplayName = "Empty"),
	BasicSet UMETA(DisplayName = "BasicSet"),
	DebugCardSet UMETA(DisplayName = "DebugCardSet"),
	EmptySet UMETA(DisplayName = "EmptySet"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardType : uint8
{
	None UMETA(DisplayName = "None"),
	Hero UMETA(DisplayName = "Hero"),
	Creature UMETA(DisplayName = "Creature"),
	Spell UMETA(DisplayName = "Spell"),
	Trap UMETA(DisplayName = "Trap"),
	Aura UMETA(DisplayName = "Aura"),
	Booster UMETA(DisplayName = "Booster"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardUseState : uint8
{
	RemainInPlay UMETA(DisplayName = "RemainInPlay"),
	SingleUse_SendToGraveyardBeforeAbility UMETA(DisplayName = "SingleUse-SendToGraveyardBeforeAbility"),
	SingleUse_SendToGraveyardAfterAbility UMETA(DisplayName = "SingleUse-SendToGraveyardAfterAbility"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardWidgetMovementState : uint8
{
	Pending UMETA(DisplayName = "Pending"),
	MoveToHandDestination UMETA(DisplayName = "MoveToHandDestination"),
	MoveToPreviewDestination UMETA(DisplayName = "MoveToPreviewDestination"),
	ReturnFromPreviewDestination UMETA(DisplayName = "ReturnFromPreviewDestination"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECostType : uint8
{
	Mana UMETA(DisplayName = "Mana"),
	TurnPoint UMETA(DisplayName = "TurnPoint"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Basic UMETA(DisplayName = "Basic"),
	Poison UMETA(DisplayName = "Poison"),
	Stun UMETA(DisplayName = "Stun"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Startup UMETA(DisplayName = "Startup"),
	MainMenu UMETA(DisplayName = "MainMenu"),
	LoadingScreen UMETA(DisplayName = "LoadingScreen"),
	Playing UMETA(DisplayName = "Playing"),
	Store UMETA(DisplayName = "Store"),
	DeckBuildingMenu UMETA(DisplayName = "DeckBuildingMenu"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EGameTurn : uint8
{
	TurnActive UMETA(DisplayName = "TurnActive"),
	TurnInactive UMETA(DisplayName = "TurnInactive"),
	Waiting UMETA(DisplayName = "Waiting..."),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EInteractionConditions : uint8
{
	None UMETA(DisplayName = "None"),
	CardCanOnlyReceiveInteractionOnOwnedPlacement UMETA(DisplayName = "CardCanOnlyRecieveInteractionOnOwnedPlacement"),
	CardCanOnlyReceiveInteractionOnAnOpponentsPlacement UMETA(DisplayName = "CardCanOnlyRecieveInteractionOnAnOppenentsPlacement"),
	CanOnlyAttackCreatures UMETA(DisplayName = "CanOnlyAttackCreatures"),
	CannotAttackIfPlayerOwnsCard UMETA(DisplayName = "CannotAttackIfPlayerOwnsCard"),
	CanReceiveSelfInteraction_AIOnly UMETA(DisplayName = "CanRecieveSelfInteraction(AIOnly)"),
	CanReceiveSelfInteractionIfDoesNotRemoveCardFromPlay_AIOnly UMETA(DisplayName = "CanRecieveSelfInteractionIfDoesNotRemoveCardFromPlay_AIOnly(AIOnly)"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EManaAccount : uint8
{
	None UMETA(DisplayName = "None"),
	Placement UMETA(DisplayName = "Placement"),
	Attack UMETA(DisplayName = "Attack"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardPlayers : uint8
{
	Self UMETA(DisplayName = "Self"),
	Opponent UMETA(DisplayName = "Opponent"),
	SelfAndOpponent UMETA(DisplayName = "SelfAndOpponent"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardPlayerState : uint8
{
	PendingAction UMETA(DisplayName = "PendingAction"),
	CardInteraction UMETA(DisplayName = "CardInteraction"),
	PlacingCard UMETA(DisplayName = "PlacingCard"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ETurnState : uint8
{
	TurnActive UMETA(DisplayName = "TurnActive"),
	TurnInactive UMETA(DisplayName = "TurnInactive"),
	UpdateCurrentState UMETA(DisplayName = "UpdateCurrentState"),
	Max UMETA(Hidden)
};
#pragma endregion

#pragma region MenuEnum
UENUM(BlueprintType)
enum class EArenaList : uint8
{
	Empty UMETA(DisplayName = "Test"),
	ArenaClassic UMETA(DisplayName = "Arena_Classic"),
	ArenaDoubles UMETA(DisplayName = "Arena_Doubles"),
	ArenaEaglesNest UMETA(DisplayName = "Arena_EaglesNest"),
	ArenaFlatGrounds UMETA(DisplayName = "Arena_FlatGrounds"),
	ArenaGreatWall UMETA(DisplayName = "Arena_GreatWall"),
	ArenaRainyDay UMETA(DisplayName = "Arena_RainyDay"),
	ArenaStoneGarden UMETA(DisplayName = "Arena_StoneGarden"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ELibraryCardSelectableState : uint8
{
	None UMETA(DisplayName = "None"),
	Selectable UMETA(DisplayName = "Selectable"),
	PreviewSelectable UMETA(DisplayName = "PreviewSelectable"),
	Locked UMETA(DisplayName = "Locked"),
	NotSelectable UMETA(DisplayName = "NotSelectable"),
	CannotAddToDeck UMETA(DisplayName = "CannotAddToDeck"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMainMenuState : uint8
{
	MainMenu UMETA(DisplayName = "MainMenu"),
	PlayOptions UMETA(DisplayName = "PlayOptions"),
	DeckSelection UMETA(DisplayName = "DeckSelection"),
	CreateGame UMETA(DisplayName = "CreateGame"),
	ArenaSelection UMETA(DisplayName = "ArenaSelection"),
	Join_FindGame UMETA(DisplayName = "Join/FindGame"),
	DeckBuilder UMETA(DisplayName = "DeckBuilder"),
	Store UMETA(DisplayName = "Store"),
	GameOptions UMETA(DisplayName = "GameOptions"),
	Max UMETA(Hidden)
};
#pragma endregion

#pragma region SystemEnum
UENUM(BlueprintType)
enum class EAITurnState : uint8
{
	Initialization UMETA(DisplayName = "Initialization"),
	Execution UMETA(DisplayName = "Execution"),
	Termination UMETA(DisplayName = "Termination"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAlphabeticalSorting : uint8
{
	AToZ UMETA(DisplayName = "AToZ"),
	ZToA UMETA(DisplayName = "ZToA"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardRowOptions : uint8
{
	None UMETA(DisplayName = "None"),
	Create UMETA(DisplayName = "Create+"),
	Save UMETA(DisplayName = "Save"),
	Edit UMETA(DisplayName = "Edit"),
	Load UMETA(DisplayName = "Load"),
	Clear UMETA(DisplayName = "Clear"),
	Cancel UMETA(DisplayName = "Cancel"),
	Delete UMETA(DisplayName = "Delete"),
	Select UMETA(DisplayName = "Select"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECardDirection : uint8
{
	Left UMETA(DisplayName = "Left"),
	CenterLeft UMETA(DisplayName = "CenterLeft"),
	CenterRight UMETA(DisplayName = "CenterRight"),
	Right UMETA(DisplayName = "Right"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEndGameResults : uint8
{
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat"),
	Draw UMETA(DisplayName = "Draw"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EErrors : uint8
{
	ValidDrop UMETA(DisplayName = "ValidDrop"),
	NotAValidPlacement UMETA(DisplayName = "NotAValidPlacement"),
	CastFailedToPlayer UMETA(DisplayName = "CastFailedToPlayer"),
	PlacementFullORNotValidPlayerPlacement UMETA(DisplayName = "PlacementFullORNotValidPlayerPlacement"),
	PlacementFull UMETA(DisplayName = "PlacementFull"),
	NotEnoughMana UMETA(DisplayName = "NotEnoughMana"),
	MaxCardsAllowedForPlacement UMETA(DisplayName = "MaxCardsAllowedForPlacement"),
	PlacementDoesNotSupportCardType UMETA(DisplayName = "PlacementDoesNotSupportCardType"),
	IncorrectPlayerPlacementForCard UMETA(DisplayName = "IncorrectPlayerPlacementForCard"),
	InvalidPlacement UMETA(DisplayName = "InvalidPlacement"),
	NoSupportedDropLocationFound UMETA(DisplayName = "NoSupportedDropLocationFound"),
	CardNotFoundInHand UMETA(DisplayName = "CardNotFoundInHand(Server)"),
	OneIsInvalid UMETA(DisplayName = "OneIsInvalid"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EManaCostSorting : uint8
{
	LowestToHighest UMETA(DisplayName = "LowestToHighest"),
	HighestToLowest UMETA(DisplayName = "HighestToLowest"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlatform : uint8
{
	Windows UMETA(DisplayName = "Windows"),
	iOS UMETA(DisplayName = "iOS"),
	Android UMETA(DisplayName = "Android"),
	HTML5 UMETA(DisplayName = "HTML5"),
	Mac UMETA(DisplayName = "Mac"),
	Linux UMETA(DisplayName = "Linux"),
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EPlatform, EPlatform::Max);

UENUM(BlueprintType)
enum class EPriority : uint8
{
	None UMETA(DisplayName = "None"),
	Low UMETA(DisplayName = "Low"),
	Medium UMETA(DisplayName = "Medium"),
	High UMETA(DisplayName = "High"),
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESortingTypeFilter : uint8
{
	Name UMETA(DisplayName = "Name"),
	ManaCost UMETA(DisplayName = "ManaCost"),
	Max UMETA(Hidden)
};
#pragma endregion
