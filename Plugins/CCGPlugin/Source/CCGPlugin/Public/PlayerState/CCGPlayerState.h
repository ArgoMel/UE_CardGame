// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "GameFramework/PlayerState.h"
#include "CCGPlayerState.generated.h"

class ACard3D;
class ACCGMode;
class ACCGPlayerController;

UCLASS()
class CCGPLUGIN_API ACCGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACCGPlayerState();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", ReplicatedUsing="OnRep_CardsInHandArray")
	TArray<UUserWidget*> mCardsInHandArray;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TObjectPtr<ACCGMode> mGameMode;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	int32 mMaxCardsInDeck;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
	bool bForceUIUpdateOnReplication;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TObjectPtr<ACCGPlayerController> mOwningPlayerController;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
	int32 mCardGamePlayerId;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attributes", ReplicatedUsing="OnRep_PlayerStat")
	FPlayerStat mPlayerStat;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerStat();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_CardsInHandArray();
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitAttributes();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitMana();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitHealth();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayersCardStates(int32 CardsInHand, int32 CardsInDeck, int32 ActiveCards);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetMana(int32 Mana);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	ACard3D* CreatePlayableCard(FTransform SpawnTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	bool AddCardToPlayersHand(FName CardName);

	UFUNCTION(Server,Reliable)
	void Server_NotifyPlayerStateChange();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_ForceUpdateUI();

	FORCEINLINE int32 GetCardGamePlayerId() const { return mCardGamePlayerId; }
};
