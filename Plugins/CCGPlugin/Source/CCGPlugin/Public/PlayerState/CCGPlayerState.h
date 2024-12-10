// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CCGPlayerState.generated.h"

UCLASS()
class CCGPLUGIN_API ACCGPlayerState : public APlayerState
{
	GENERATED_BODY()
	public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitAttributes();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitMana();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerHealth();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitHealth();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_CardsInHand_Array();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayersCardStates(int32 CardsInHand, int32 CardsInDeck, int32 ActiveCards);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_CardsInDeck();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_ActiveCards();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_CardsInHand();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Mana();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void Set_Mana(int32 Mana);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerTurn();

	/** Please add a function description */
	//UFUNCTION(BlueprintCallable)
	//void CreatePlayableCard(FTransform SpawnTransform, A3DCard_C*& CardCreated);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void AddCardToPlayersHand(FName CardName, bool& Success);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Mana_Max();
public:
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	// TObjectPtr<USceneComponent> DefaultSceneRoot;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, Mana) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_Mana")
	// int32 Mana;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, Mana_Max) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_Mana_Max")
	// int32 Mana_Max;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, Health) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_Health")
	// int32 Health;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, CardGamePlayerId) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
	// int32 CardGamePlayerId;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes")
	// int32 Health_Max;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, CardsInHand) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_CardsInHand")
	// int32 CardsInHand;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, PlayerTurn) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", ReplicatedUsing="OnRep_PlayerTurn")
	// int32 PlayerTurn;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, CardsInHand_Array) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", ReplicatedUsing="OnRep_CardsInHand_Array")
	// TArray<UCardWidget_C*> CardsInHand_Array;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, CardsInDeck) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_CardsInDeck")
	// int32 CardsInDeck;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, ActiveCards) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_ActiveCards")
	// int32 ActiveCards;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, OwningPlayerController) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
	// TObjectPtr<ABP_CardGamePlayerController_C> OwningPlayerController;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	// TObjectPtr<ABP_CardGameMode_C> GameMode_Ref;
	//
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	// int32 MaxCardsInDeck;
	//
	// /** Please add a variable description */
	// static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGamePlayerState, ForceUIUpdateOnReplication) to GetLifetimeReplicatedProps");
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
	// bool ForceUIUpdateOnReplication;

};
