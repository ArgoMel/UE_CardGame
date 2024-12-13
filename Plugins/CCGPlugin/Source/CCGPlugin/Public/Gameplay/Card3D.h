// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card3D.generated.h"

class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class CCGPLUGIN_API ACard3D : public AActor
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
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, CardDataStruct) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// FCard_Struct CardDataStruct;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, CardDataTableName) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// FName CardDataTableName;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Name) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// FText Name;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Attack")
	int32 mAttack;

	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Health) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Health")
	// int32 Health;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Preview")
	bool PreviewEnabled;

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
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, PlacementOwner) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	// TObjectPtr<ACardPlacement_C> PlacementOwner;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// TObjectPtr<A3DCard_C> DamageDealingCard_Ref;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, OwningPlayer) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	// TObjectPtr<ABP_CardGamePlayerController_C> OwningPlayer;
	//
	// /** Has the card been placed on the board? */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, bIsPlaced) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	// bool bIsPlaced;
	//
	// /** Is the card selected by the player */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// bool bIsSelected;
	//
	// /** Whether the card is currrently attacking */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	// bool bIsAttacking;
	//
	// /** Has the card been moved to the player graveyard */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, bInGraveyard) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	// bool bInGraveyard;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, TurnPoints) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// int32 TurnPoints;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, LifeExpectancy) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// int32 LifeExpectancy;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, TurnsAlive) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_TurnsAlive")
	// int32 TurnsAlive;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Type) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", ReplicatedUsing="OnRep_Type")
	// TEnumAsByte<CardType_Enum> Type;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, CardSet) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// TEnumAsByte<CardSet_Enum> CardSet;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, Description) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// FString Description;
	//
	// /** The Type(s) of abilities this card posesses */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, AbilityType(s)) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities", Replicated)
	// TArray<TEnumAsByte<AbilityType_Enum> > AbilityType(s);
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	// TArray<FAbility_Struct> Ability_Struct_Refs;
	//
	// /** T */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Abilities")
	// bool IsAbilityActive;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, CanAttackPlayer) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// bool CanAttackPlayer;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, CanAttackCards) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Data", Replicated)
	// bool CanAttackCards;
	//
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
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(A3DCard, OwningPlayerID) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", ReplicatedUsing="OnRep_OwningPlayerID")
	// int32 OwningPlayerID;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mCardID;

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
	
public:
	// UFUNCTION(BlueprintCallable, Category="Gameplay")
	// void MoveCardToGravyard();
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
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// void OnRep_Mana_Cost_Placement();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Data")
	// void SetCardVisuals();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Gameplay")
	// void PlayMovementAnimation(TEnumAsByte<3DCardMovementState_Enum> CardMovementState, FVector ManualGoalDestination, FRotator ManualGoalRotation, AActor* OR_GoalActor, double Z Offset, double ErrorTolerance, double InterSpeed, AActor* NewLocalVar_0);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// void OnRep_Card Type();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="System")
	// void LogDisplayMessage(FString Message, FLinearColor Specified Color);
	//
	// /** Run the Cards ability */
	// UFUNCTION(BlueprintCallable, Category="Abilities")
	// void RunCardAbility(int32 AbilityIndex, bool& Succesful, int32 AbilityIndex_Ref);

	FORCEINLINE int32 GetAttack() const {return mAttack;}
};
