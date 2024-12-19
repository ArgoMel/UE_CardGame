// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "GameFramework/Actor.h"
#include "CardPlacement.generated.h"

class AGraveyard;
class ACard3D;
class UBoxComponent;

UCLASS()
class CCGPLUGIN_API ACardPlacement : public AActor
{
	GENERATED_BODY()
public:
	ACardPlacement();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UBoxComponent> PlacementCollision;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement", Replicated)
	TArray<ACard3D*> mCards;
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Card Placement| Settings", Replicated)
	TObjectPtr<AGraveyard> mGraveyard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	ECardDirection mFillDirection;
	/** Player which this placement belongs to. Set to '0' if both players can play cards on this placement.  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings", Replicated)
	int32 mPlayerIndex;
	/** Max Number of cards allowed in the placement */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	int32 mMaxCardsInPlacement;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	int32 mDemoCards;
	/** The number of card widths this placement will span across */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	double mPlacementWidth;
	/** Card spacing determining the minimum spacing between each of the cards. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	double mCardSpacing;
	/** Card width determines the overall width of each individual card. It is used to calculate the local size of the placement, and also specifies the maximum spacing cards can have between one another. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	double mCardWidth;
	/** Does this placement hold more than 1 card? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings", Replicated)
	bool bMultiPlacement;
	/** Scale the card placement or stack cards on top of one another */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	bool bScaleMultiCardPlacement;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Settings", Replicated)
	bool bIsFull;
	/** Dynamic spacing will dynamically change the spacing between each of the cards based on the current number of cards in the placement (Min = Card Spacing, Max = Card Width) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	bool bDynamicSpacing;
	/** Whether the cards in the placement should dynamically position themselves within the placement bounds. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Settings")
	bool bCenterOut;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Settings")
	bool bInvertPlacementDirection;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	bool bDisplayDemoCards;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bSupportsAllCardTypes;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bHero;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bCreature;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bSpell;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bTrap;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bAura;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Supported Card Types")
	bool bBooster;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Card Placement| Board Postion")
	TObjectPtr<ACardPlacement> mPlacementLeft;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Card Placement| Board Postion")
	TObjectPtr<ACardPlacement> mPlacementRight;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Board Postion")
	int32 mRowX;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Board Postion")
	int32 mRowY;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Tiles")
	TObjectPtr<UClass> mSingleTIle;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Tiles")
	TObjectPtr<UClass> mLeftEndTile;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Tiles")
	TObjectPtr<UClass> mCenterTile;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Tiles")
	TObjectPtr<UClass> mRightEndTile;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Supported Card Types")
	TArray<ECardType> mValidCardTypes;

public:
	/** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// void GetCardInPlacementLocation(int32 CardIndex, int32 MaxCards, int32 CardsInPlacement, FVector& ReturnVector, FRotator& ReturnRotation);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// void RemoveCardFromPlacement(const A3DCard_C*& CardToClear);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// void CompileSupportedCardTypes();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// void GetPlacementState(int32& PlayerIndex, bool& IsMultiCardPlacement, bool& IsFull, int32& CardsInPlacement, int32& MaxCardsInPlacement, bool& SupportAllCardTypes, TArray<TEnumAsByte<CardType_Enum> >& ValidCardTypesArray);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// void CreateVisualBoardTiles();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// double CalculateDynamicSpacingLocation(int32 Index, int32 MaxCardsInPlacement, int32 CardInPlacement, bool CenterOut, bool& Overlap);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// double CalculateSpacingLocation(int32 Index, int32 MaxCardsInPlacement, int32 CardInPlacement, double Spacing);

	UFUNCTION(Server,Reliable)
	void Server_AddCardToBoardPlacement(ACard3D* CardToAdd);

	FORCEINLINE bool IsFull() const { return bIsFull; }
	FORCEINLINE ACardPlacement* GetPlacementLeft() const { return mPlacementLeft; }
	FORCEINLINE ACardPlacement* GetPlacementRight() const { return mPlacementRight; }
	FORCEINLINE int32 GetPlayerIndex() const { return mPlayerIndex; }
};
