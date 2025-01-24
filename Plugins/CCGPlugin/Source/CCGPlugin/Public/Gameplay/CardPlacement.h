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
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TArray<UChildActorComponent*> mChildActors;
	UPROPERTY()
	TArray<UStaticMeshComponent*> mDemoMesh;
	
protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UBoxComponent> mPlacementCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TObjectPtr<UStaticMesh> mCardMesh;
	
	UPROPERTY(BlueprintReadWrite, Category="Card Placement", Replicated)
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
	/** The number of card widths this placement will span across */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	float mPlacementWidth;
	/** Card spacing determining the minimum spacing between each of the cards. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	float mCardSpacing;
	/** Card width determines the overall width of each individual card. It is used to calculate the local size of the placement, and also specifies the maximum spacing cards can have between one another. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	float mCardWidth;
	/** Scale the card placement or stack cards on top of one another */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Settings")
	bool bScaleMultiCardPlacement;
	UPROPERTY(BlueprintReadWrite, Category="Card Placement| Settings", Replicated)
	bool bIsFull;
	/** Dynamic spacing will dynamically change the spacing between each of the cards based on the current number of cards in the placement (Min = Card Spacing, Max = Card Width) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Settings")
	bool bDynamicSpacing;
	/** Whether the cards in the placement should dynamically position themselves within the placement bounds. */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement| Settings")
	bool bCenterOut;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement| Settings")
	bool bInvertPlacementDirection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Demo")
	int32 mDemoCards;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement| Demo")
	bool bDisplayDemoCards;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Card Placement| Board Postion")
	TObjectPtr<ACardPlacement> mPlacementLeft;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Card Placement| Board Postion")
	TObjectPtr<ACardPlacement> mPlacementRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Tiles")
	TSubclassOf<AActor> mSingleTIle;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Tiles")
	TSubclassOf<AActor> mLeftEndTile;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Tiles")
	TSubclassOf<AActor> mCenterTile;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement| Tiles")
	TSubclassOf<AActor> mRightEndTile;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category="Card Placement| Supported Card Types")
	TArray<ECardType> mValidCardTypes;

protected:
	void SetAllCardLocation();

public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure,Category="Card Placement")
	FTransform GetCardInPlacementLocation(int32 CardIndex, int32 MaxCards, int32 CardsInPlacement) const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,Category="Card Placement")
	void RemoveCardFromPlacement(ACard3D* CardToClear);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,Category="Card Placement")
	void CreateVisualBoardTiles();
	/** Please add a function description */
	UFUNCTION(BlueprintPure,Category="Card Placement")
	float CalculateDynamicSpacingLocation(int32 Index, int32 MaxCardsInPlacement, int32 CardInPlacement, bool CenterOut, bool& Overlap) const;
	/** Please add a function description */
	UFUNCTION(BlueprintPure,Category="Card Placement")
	float CalculateSpacingLocation(int32 Index, int32 MaxCardsInPlacement, int32 CardInPlacement, float Spacing) const;

	UFUNCTION(Server,Reliable)
	void Server_RemoveCardFromPlacement(ACard3D* CardToClear);
	UFUNCTION(Server,Reliable)
	void Server_AddCardToBoardPlacement(ACard3D* CardToAdd);

	UFUNCTION(BlueprintCallable,CallInEditor, Category="Editor")
	void FindAndSetAroundPlacement();

	FORCEINLINE bool IsFull() const { return bIsFull; }
	FORCEINLINE ACardPlacement* GetPlacementLeft() const { return mPlacementLeft; }
	FORCEINLINE ACardPlacement* GetPlacementRight() const { return mPlacementRight; }
	FORCEINLINE AGraveyard* GetGraveyard() const { return mGraveyard; }
	FORCEINLINE int32 GetPlayerIndex() const { return mPlayerIndex; }
	FORCEINLINE int32 GetMaxCardsInPlacement() const { return mMaxCardsInPlacement; }
};
