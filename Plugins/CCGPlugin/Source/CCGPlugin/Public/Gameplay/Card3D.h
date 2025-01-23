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
	FTimerHandle mMoveToGraveyardTimer;
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
	UPROPERTY(BlueprintReadWrite, Category="Misc Variables", Replicated)
	FVector mHomeDestination;
	/** The goal destination is used to move the card when performing actions away from the home destination. This can be used for fly-to animations, like when the card is attacking.  */
	UPROPERTY(BlueprintReadWrite, Category="Misc Variables")
	FVector mGoalDestination;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Misc Variables")
	FRotator mHomeRotation;
	/** The goal rotation is simply a rotation goal which the card can be set to, much like the goal destination.  */
	UPROPERTY(BlueprintReadWrite, Category="Misc Variables")
	FRotator mGoalRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	TObjectPtr<ACardPlacement> mPlacementOwner;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGPlayerController> mOwningPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGState> mGameState;
	/** Current platform the game is being played on */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<UCCGameInstance> mGameInstance;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACard3D> mDamageDealingCard;
	/** The movement state which the card is currently moving within. This determines the result when the movement is completed, and whether to trigger additional evens or functions.  */
	UPROPERTY(BlueprintReadWrite, Category="System")
	E3DCardMovementState mCardMovementState;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	int32 mOwningPlayerID;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	int32 mCardID;
	/** Interpolation Speed */
	UPROPERTY(BlueprintReadWrite, Category="System")
	float mInterpSpeed;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	float mErrorTolerance;
	/** Has the card been placed on the board? */
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	bool bIsPlaced;
	/** Is the card selected by the player */
	UPROPERTY(BlueprintReadWrite, Category="System")
	bool bIsSelected;
	/** Whether the card is currently attacking */
	UPROPERTY(BlueprintReadWrite, Category="System")
	bool bIsAttacking;
	/** Has the card been moved to the player graveyard */
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	bool bInGraveyard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System", ReplicatedUsing="OnRep_bCardActive")
	bool bCardActive;
	
	UPROPERTY(BlueprintReadWrite, Category="Card Data", ReplicatedUsing="OnRep_Stat")
	FCard mCardData;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Data", Replicated)
	FName mCardDataTableName;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Data", Replicated)
	ECardSet mCardSet;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Data", Replicated)
	int32 mTurnPoint;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Data", Replicated)
	int32 mTurnsAlive;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Preview")
	bool bPreviewEnabled;
	
	/** A mouse over card widget reference */
	UPROPERTY(BlueprintReadWrite, Category="Visual")
	TObjectPtr<UUserWidget> mViewCardPreview;
	
	/** T */
	UPROPERTY(BlueprintReadWrite, Category="Abilities")
	bool bIsAbilityActive;

private:
	void ShowPreviewCard();
	
protected:
	UFUNCTION()
	void OnRep_Stat();
	UFUNCTION()
	void OnRep_bCardActive();

	UFUNCTION(Category="Delegate")
	void OnCardBeginOverlap(AActor* TouchedActor);
	UFUNCTION(Category="Delegate")
	void OnCardEndOverlap(AActor* TouchedActor);

	UFUNCTION(Category="Timer")
	void ShowPreviewCardTimer();
	UFUNCTION(Category="Timer")
	void StartDamageCardFlash();
	UFUNCTION(Category="Timer")
	void StartAbilityCardFlash();
	UFUNCTION(Category="Timer")
	void RemoveCardActor();

	UFUNCTION(Category="Macro")
	void ChangeTurnStateVisual(ETurnState CardGameState) const;
	UFUNCTION(Category="Macro")
	void RemoveMouseOverPreview();
	UFUNCTION(Category="Macro")
	bool ValidateAbility() const;
	UFUNCTION(Category="Macro")
	bool RunActiveAbility(EAbilityTrigger AbilityTrigger);
	UFUNCTION(Category="Macro")
	bool CheckAbilityUseState(EAbilityTrigger CheckAbilityTrigger,ECardUseState CheckCardUseState,int32 AbilityIndex, bool SearchAllAbilities);
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Data")
	void SetTurnStateVisual(bool IsActive) const;
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
	void GenerateDescription(FString DescriptionText) const;
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Card Location and Rotation")
	FRotator InterpSelfToRotation(float DeltaTime,bool& Arrived) const;
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Card Location and Rotation")
	FVector InterpSelfToLocation(float DeltaTime,bool& Arrived) const;
	
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
	void Multicast_Attacking(AActor* Attacking);
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
	FORCEINLINE ACard3D* GetDamageDealingCard()  {return mDamageDealingCard;}
	FORCEINLINE FCard* GetCardData()  {return &mCardData;}
	FORCEINLINE FName GetCardDataTableName() const {return mCardDataTableName;}
	FORCEINLINE int32 GetCardID() const {return mCardID;}
	FORCEINLINE int32 GetTurnPoint() const {return mTurnPoint;}
	FORCEINLINE int32 GetOwningPlayerID() const {return mOwningPlayerID;}
	FORCEINLINE ECardSet GetCardSet() const {return mCardSet;}
	FORCEINLINE bool IsInGraveyard() const {return bInGraveyard;}
	FORCEINLINE bool IsPlaced() const {return bIsPlaced;}
	FORCEINLINE bool IsPreviewEnabled() const {return bPreviewEnabled;}

	FORCEINLINE void SetPlacementOwner(ACardPlacement* PlacementOwner) {mPlacementOwner=PlacementOwner;}
	FORCEINLINE void SetDamageDealingCard(ACard3D* Card) {mDamageDealingCard=Card;}
	FORCEINLINE void SetAttack(int32 Attack) {mCardData.Attack.Damage=Attack;}
	FORCEINLINE void SetCardID(int32 ID) {mCardID=ID;}
	FORCEINLINE void SetTurnPoint(int32 Point) {mTurnPoint=Point;}
	FORCEINLINE void SetOwningPlayerID(int32 ID) {mOwningPlayerID=ID;}
	FORCEINLINE void SetIsAbilityActive(bool AbilityActive){bIsAbilityActive=AbilityActive;}
};
