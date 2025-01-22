// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoardPlayer.h"
#include "Common/CCGStruct.h"
#include "GameFramework/Actor.h"
#include "Interface/CardInteractionInterface.h"
#include "Card3D.generated.h"

class UCCGameInstance;
class ACCGState;
class ACCGPlayerController;
class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class CCGPLUGIN_API ACard3D : public AActor
	,public ICardInteractionInterface
{
	GENERATED_BODY()
public:
	ACard3D();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void TakesDamage_Implementation(ACard3D* DealingCard, int32 DamageAttack, int32 DamageHealth) override;
	virtual void OnActivePlayerTurn_Implementation() override;
	virtual void OnEndActivePlayerTurn_Implementation() override;

private:
	bool bUseGate;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UBoxComponent> mBox;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mNameText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mCardTypeText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mDescriptionText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mCardBaseMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mHeartMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mHealthText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mManaGemMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mManaCostText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mAttackMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mAttackText;
	
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TSubclassOf<UUserWidget> mCardPreviewClass;
	
	/** Home Destination a variable which holds the placement destination of the card. This is where the card will always end up if it is still in play.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables", Replicated)
	FVector mHomeDestination;
	/** The goal destination is used to move the card when performing actions away from the home destination. This can be used for fly-to animations, like when the card is attacking.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables")
	FVector mGoalDestination;
	/** The goal rotation is simply a rotation goal which the card can be set to, much like the goal destination.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables")
	FRotator mGoalRotation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables")
	FRotator mHomeRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	TObjectPtr<ACardPlacement> mPlacementOwner;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGPlayerController> mOwningPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGState> mGameState;
	/** Current platform the game is being played on */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<UCCGameInstance> mGameInstance;
	/** The movement state which the card is currently moving within. This determines the result when the movement is completed, and whether to trigger additional evens or functions.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	E3DCardMovementState mCardMovementState;
	/** The maximum amount of characters that the 3D text renderer can hold per line. Note: This does not currently filter full words. Which may result in words being split between lines.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mMaxDescCharacterLineLength;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	int32 mOwningPlayerID;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mCardID;
	/** Interpolation Speed */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	double mInterpSpeed;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	double mErrorTolerance;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	double mCardPreviewScale;
	/** Has the card been placed on the board? */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	bool bIsPlaced;
	/** Is the card selected by the player */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bIsSelected;
	/** Whether the card is currently attacking */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bIsAttacking;
	/** Has the card been moved to the player graveyard */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	bool bInGraveyard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	bool bEnableLifeExpectancy;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", ReplicatedUsing="OnRep_bCardActive")
	bool bCardActive;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	FCard mCardData;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	FName mCardDataTableName;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	FText mName;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	FString mDescription;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_CardType")
	ECardType mType;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	ECardSet mCardSet;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Stat")
	int32 mAttack;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Stat")
	int32 mHealth;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Stat")
	int32 mManaCostPlacement;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	int32 mTurnPoint;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	int32 mLifeExpectancy;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_TurnsAlive")
	int32 mTurnsAlive;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	bool bCanAttackPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	bool bCanAttackCard;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Preview")
	bool bPreviewEnabled;
	
	/** Reference to Shadow material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterial> mShadowMaterial;
	/** A mouse over card widget reference */
	UPROPERTY(BlueprintReadWrite, Category="Visual")
	TObjectPtr<UUserWidget> mViewCardPreview;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities", Replicated)
	TArray<EAbilityTrigger> mAbilityTriggers;
	/** The Type(s) of abilities this card possesses */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities", Replicated)
	TArray<EAbilityType> mAbilityTypes;
	/** T */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	bool bIsAbilityActive;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACard3D> mDamageDealingCard;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	TArray<FCardAbility> mAbilities;

private:
	void ShowPreviewCard();
	
protected:
	UFUNCTION()
	void OnRep_Stat();
	UFUNCTION()
	void OnRep_TurnsAlive();
	UFUNCTION()
	void OnRep_bCardActive();
	UFUNCTION()
	void OnRep_CardType();

	UFUNCTION(Category="Delegate")
	void OnCardBeginOverlap(AActor* TouchedActor);
	UFUNCTION(Category="Delegate")
	void OnCardEndOverlap(AActor* TouchedActor);

	UFUNCTION(Category="Timer")
	void ShowPreviewCardTimer();

	UFUNCTION(Category="Macro")
	void ChangeTurnStateVisual(ETurnState CardGameState) const;
	UFUNCTION(Category="Macro")
	void RemoveMouseOverPreview();
	UFUNCTION(Category="Macro")
	void SaveCardStateToStruct();
	UFUNCTION(Category="Macro")
	bool ValidateAbility() const;
	UFUNCTION(Category="Macro")
	bool RunActiveAbility(EAbilityTrigger AbilityTrigger);
	UFUNCTION(Category="Macro")
	bool CheckAbilityUseState(EAbilityTrigger CheckAbilityTrigger,ECardUseState CheckCardUseState,int32 AbilityIndex, bool SearchAllAbilities);
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Data")
	void SetTurnStateVisual(bool IsActive);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Data")
	void SetCardData(FName CardTableName, ECardSet CardSet);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Data")
	void SetCardVisuals();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void MoveCardToGraveyard();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void PlayMovementAnimation(E3DCardMovementState CardMovementState, FVector ManualGoalDestination, FRotator ManualGoalRotation, AActor* OR_GoalActor=nullptr, float ZOffset=0.f, float ErrorTolerance=0.f, float InterpSpeed=1.f);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Description Generator")
	void GenerateDescription(FString DescriptionText);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Card Location and Rotation")
	FRotator InterpSelfToRotation(bool& Arrived);
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Card Location and Rotation")
	FVector InterpSelfToLocation(bool& Arrived);
	
	/** Run the Cards ability */
	UFUNCTION(BlueprintCallable, Category="Abilities")
	bool RunCardAbility(int32 AbilityIndex);

	UFUNCTION(Category="Event")
	void SpawnEffect(ECardEffects CardEffect);
	UFUNCTION(Category="Event")
	void OpenGate();
	UFUNCTION(Category="Event")
	void CloseGate();
	UFUNCTION(Category="Event")
	void ArrivedAtDestination();
	UFUNCTION(Category="Event")
	void SetClientSideData(FName CardTableName,ECardSet CardRace);

	UFUNCTION(Category="Event|Selection")
	void Selected(int32 SelectingPlayerID);
	UFUNCTION(Category="Event|Selection")
	void Deselected();
	
	UFUNCTION(Category="Event|Card Preview")
	void EnableMobileCardPreview();
	UFUNCTION(Category="Event|Card Preview")
	void DisableMobileCardPreview();

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_UpdateCardVisual(bool IsVisual);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_UpdateVisualStats();
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_SpawnEffect(ECardEffects CardEffect);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetCardHiddenState(bool IsHidden);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlaceCardOnBoard(const FTransform& Destination);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_Attacking(AActor* board_player);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_MoveToGraveyard();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ForceMoveCardDirectlyToGraveyard(AGraveyard* Graveyard);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetCardVisuals();
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_DamageVisual();
	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_AbilityVisual();
	
	UFUNCTION(Server,Reliable)
	void Server_SetCardData(const FName& CardTableName,ECardSet CardRace);
	UFUNCTION(Server,Reliable)
	void Server_ActivateAbility(int32 AbilityIndex);
	UFUNCTION(Server,Reliable)
	void Server_RemoveCardActor();
	
	FORCEINLINE ACardPlacement* GetPlacementOwner()  {return mPlacementOwner;}
	FORCEINLINE FCard* GetCardData()  {return &mCardData;}
	FORCEINLINE FName GetCardDataTableName() const {return mCardDataTableName;}
	FORCEINLINE int32 GetHealth() const {return mHealth;}
	FORCEINLINE int32 GetAttack() const {return mAttack;}
	FORCEINLINE int32 GetCardID() const {return mCardID;}
	FORCEINLINE int32 GetTurnPoint() const {return mTurnPoint;}
	FORCEINLINE int32 GetOwningPlayerID() const {return mOwningPlayerID;}
	FORCEINLINE ECardSet GetCardSet() const {return mCardSet;}
	FORCEINLINE ECardType GetCardType() const {return mType;}
	FORCEINLINE bool IsInGraveyard() const {return bInGraveyard;}
	FORCEINLINE bool IsPlaced() const {return bIsPlaced;}
	FORCEINLINE bool IsPreviewEnabled() const {return bPreviewEnabled;}
	FORCEINLINE bool CanAttackPlayer() const {return bCanAttackPlayer;}
	FORCEINLINE bool CanAttackCard() const {return bCanAttackCard;}

	FORCEINLINE void SetPlacementOwner(ACardPlacement* PlacementOwner) {mPlacementOwner=PlacementOwner;}
	FORCEINLINE void SetAttack(int32 Attack) {mAttack=Attack;}
	FORCEINLINE void SetCardID(int32 ID) {mCardID=ID;}
	FORCEINLINE void SetTurnPoint(int32 Point) {mTurnPoint=Point;}
	FORCEINLINE void SetOwningPlayerID(int32 ID) {mOwningPlayerID=ID;}
	FORCEINLINE void SetIsAbilityActive(bool AbilityActive){bIsAbilityActive=AbilityActive;}
};
