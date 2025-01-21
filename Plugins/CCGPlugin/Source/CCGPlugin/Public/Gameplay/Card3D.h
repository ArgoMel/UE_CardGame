// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoardPlayer.h"
#include "Common/CCGStruct.h"
#include "GameFramework/Actor.h"
#include "Interface/CardInteractionInterface.h"
#include "Card3D.generated.h"

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

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mNameText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mCardTypeText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mDescriptionText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mAttackText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mAttackMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mHealthText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mHeartMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mManaCostText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mManaGemMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mCardBaseMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UBoxComponent> mBox;

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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGPlayerController> mOwningPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGState> mGameState;
	/** Current platform the game is being played on */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	EPlatform mPlatform;
	/** The movement state which the card is currently moving within. This determines the result when the movement is completed, and whether to trigger additional evens or functions.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	E3DCardMovementState mCardMovementState;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	ETurnState mCardGameState;
	/** The maximum amount of characters that the 3D text renderer can hold per line. Note: This does not currently filter full words. Which may result in words being split between lines.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mMaxDescCharacterLineLength;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", ReplicatedUsing="OnRep_OwningPlayerID")
	int32 mOwningPlayerID;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mCardID;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	double mDeltaTime;
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bIsPlatformMobile;
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Attack")
	int32 mAttack;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Health")
	int32 mHealth;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_ManaCostPlacement")
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

	/** This is an extra layer around the card which can be set to display anything like an outline or a shadow without needing any complex material setup. Mostly used for optimisation purposes.   */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterial> mOuterMaterial;
	/** Reference to Shadow material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterial> mShadowMaterial;
	/** The cards frame material.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstance> mCardFrameMaterial;
	/** Card Frame Material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstanceDynamic> mCardFrameMaterial_Instance;
	/** The cards image material.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstance> mCardImageMaterial;
	/** Card Image Material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstanceDynamic> mCardImageMaterial_Instance;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstanceDynamic> mCardBackMaterial;
	/** A mouse over card widget reference */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	TArray<FCardAbility> mAbilities;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACard3D> mDamageDealingCard;

protected:
	/** Please add a function description */
	UFUNCTION()
	void OnRep_Attack();
	
	/** Please add a function description */
	UFUNCTION()
	void OnRep_Health();
	
	/** Please add a function description */
	UFUNCTION()
	void OnRep_TurnsAlive();
	
	/** Please add a function description */
	UFUNCTION()
	void OnRep_bCardActive();
	
	/** Please add a function description */
	UFUNCTION()
	void OnRep_OwningPlayerID();

	/** Please add a function description */
	UFUNCTION()
	void OnRep_ManaCostPlacement();
	
	/** Please add a function description */
	UFUNCTION()
	void OnRep_CardType();
	
public:
	// UFUNCTION(BlueprintCallable, Category="Gameplay")
	// void MoveCardToGraveyard();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Description Generator")
	// void GenerateDescription(FString Description Text, int32 CurrentString Character);

	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Data")
	// void SetTurnStateVisual(bool IsActive);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="Card Location and Rotation")
	// FRotator InterpSelfToRotation(bool& Arrived);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="Card Location and Rotation")
	// FVector InterpSelfToLocation(bool& Arrived);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Data")
	// void SetCardData(FName CardTableName, TEnumAsByte<CardSet_Enum> CardSet, FCard_Struct CardStruct, bool SetCardDataFromStruct);

	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Data")
	// void SetCardVisuals();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Gameplay")
	// void PlayMovementAnimation(TEnumAsByte<3DCardMovementState_Enum> CardMovementState, FVector ManualGoalDestination, FRotator ManualGoalRotation, AActor* OR_GoalActor, double Z Offset, double ErrorTolerance, double InterSpeed, AActor* NewLocalVar_0);

	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="System")
	// void LogDisplayMessage(FString Message, FLinearColor Specified Color);
	//
	// /** Run the Cards ability */
	// UFUNCTION(BlueprintCallable, Category="Abilities")
	// void RunCardAbility(int32 AbilityIndex, bool& Successful, int32 AbilityIndex_Ref);

	UFUNCTION(Category="Event")
	void SetClientSideData(FName CardTableName,ECardSet CardRace);
	UFUNCTION(Category="Event")
	void EnableMobileCardPreview();
	UFUNCTION(Category="Event")
	void DisableMobileCardPreview();
	UFUNCTION(Category="Event")
	void Selected(int32 SelectingPlayerID);
	UFUNCTION(Category="Event")
	void Deselected();

	UFUNCTION(Server,Reliable)
	void Server_SetCardData(const FName& CardTableName,ECardSet CardRace);
	UFUNCTION(Server,Reliable)
	void Server_RemoveCardActor();
	
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_Attacking(AActor* board_player);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_UpdateCardVisual(bool IsVisual);
	
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_UpdateVisualStats();

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ForceMoveCardDirectlyToGraveyard(AGraveyard* Graveyard);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetCardHiddenState(bool IsHidden);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlaceCardOnBoard(const FTransform& Destination);

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
