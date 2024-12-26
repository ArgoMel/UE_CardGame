// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoardPlayer.h"
#include "Common/CCGStruct.h"
#include "GameFramework/Actor.h"
#include "Interface/CardInteractionInterface.h"
#include "Card3D.generated.h"

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
	TObjectPtr<UTextRenderComponent> Name_Text;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> CardType_Text;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> Description_Text;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> Attack_Text;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> Attack_Plane;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> Health_Text;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> Heart_Plane;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> ManaCost_Text;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> ManaGem_Plane;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> CardBase;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UBoxComponent> Box;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	double mDeltaTime;

	/** Home Destination a variable which holds the placement destination of the card. This is where the card will always end up if it is still in play.  */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, HomeDestination) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables", Replicated)
	// FVector HomeDestination;
	//
	// /** The goal destination is used to move the card when performing actions away from the home destination. This can be used for fly-to animations, like when the card is attacking.  */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables")
	// FVector GoalDestination;
	//
	// /** The goal rotation is simply a rotation goal which the card can be set to, much like the goal destination.  */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables")
	// FRotator GoalRotation;
	//
	// /** Interpolation Speed */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// double InterpSpeed;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// double ErrorTolerance;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// double CardPreviewScale;
	//
	// /** The maximum amount of characters that the 3D text renderer can hold per line. Note: This does not currently filter full words. Which may result in words being split between lines.  */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// int32 MaxDescriptionCharacterLineLength;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	FCard mCardData;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	FName mCardDataTableName;
	
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Name) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// FText Name;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Attack")
	int32 mAttack;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Health")
	int32 mHealth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Preview")
	bool bPreviewEnabled;

	/** This is an extra layer around the card which can be set to display anything like an outline or a shadow without needing any complex material setup. Mostly used for optimisation purposes.   */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterial> OuterMaterial;

	/** Reference to Shadow material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterial> Shadow_Mat_Ref;

	/** The cards frame material.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstance> CardFrameMaterial;

	/** Card Frame Material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstanceDynamic> CardFrameMaterial_Instance;

	/** The cards image material.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstance> CardImageMaterial;

	/** Card Image Material */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	TObjectPtr<UMaterialInstanceDynamic> CardImageMaterial_Instance;

	// /** A mouse over card widget reference */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual", meta=(EditInline="true"))
	// TObjectPtr<UCardPreview_C> ViewCardPreview;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, AbilityTrigger(s)) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities", Replicated)
	// TArray<TEnumAsByte<AbilityTrigger_Enum> > AbilityTrigger(s);
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Mana_Cost_Placement) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Mana_Cost_Placement")
	// int32 Mana_Cost_Placement;
	//
	// /** Current platform the game is being played on */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// TEnumAsByte<Platform_Enum> Platform;
	//
	// /** The movement state which the card is currently moving within. This determins the result when the movement is completed, and whether to trigger additional evens or functions.  */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// TEnumAsByte<3DCardMovementState_Enum> CardMovementState;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// TEnumAsByte<TurnState_Enum> CardGameState;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	TObjectPtr<ACardPlacement> mPlacementOwner;
	
	/** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, OwningPlayer) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	// TObjectPtr<ABP_CardGamePlayerController_C> OwningPlayer;
	
	/** Has the card been placed on the board? */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	bool bIsPlaced;
	
	// /** Is the card selected by the player */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// bool bIsSelected;
	//
	// /** Whether the card is currrently attacking */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// bool bIsAttacking;
	
	/** Has the card been moved to the player graveyard */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	bool bInGraveyard;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	int32 mTurnPoint;
	
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, LifeExpectancy) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// int32 LifeExpectancy;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, TurnsAlive) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_TurnsAlive")
	// int32 TurnsAlive;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_CardType")
	ECardType mType;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	ECardSet mCardSet;
	
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Description) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// FString Description;
	//
	// /** The Type(s) of abilities this card posesses */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, AbilityType(s)) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities", Replicated)
	// TArray<TEnumAsByte<AbilityType_Enum> > AbilityType(s);
	
	/** T */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	bool bIsAbilityActive;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	bool bCanAttackPlayer;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	bool bCanAttackCard;
	
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// bool bIsPlatformMobile;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, bEnableLifeExpectancy) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	// bool bEnableLifeExpectancy;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc Variables")
	// FRotator HomeRotation;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Visual")
	// TObjectPtr<UMaterialInstanceDynamic> CardBackMaterial;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, bCardActive) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", ReplicatedUsing="OnRep_bCardActive")
	// bool bCardActive;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// TObjectPtr<ABP_CardGameState_C> GameState_Ref;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", ReplicatedUsing="OnRep_OwningPlayerID")
	int32 mOwningPlayerID;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mCardID;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	TArray<FCardAbility> mAbilities;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACard3D> mDamageDealingCard;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Attack();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Health();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_TurnsAlive();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_bCardActive();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_OwningPlayerID();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_ManaCostPlacement();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
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
	void Server_SetCardData(const FName& CardTableName,ECardSet CardRace,FCard CardStruct,bool SetCardDataFromStruct);
	
	UFUNCTION(NetMulticast,Reliable)
	void MultiCast_Attacking(AActor* board_player);

	UFUNCTION(NetMulticast,Reliable)
	void MultiCast_UpdateCardVisual(bool IsVisual);
	
	UFUNCTION(NetMulticast,Reliable)
	void MultiCast_UpdateVisualStats();

	UFUNCTION(NetMulticast,Reliable)
	void ForceMoveCardDirectlyToGraveyard(AGraveyard* graveyard);

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
