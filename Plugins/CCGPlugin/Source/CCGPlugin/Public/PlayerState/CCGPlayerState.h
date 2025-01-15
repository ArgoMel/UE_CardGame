// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "GameFramework/PlayerState.h"
#include "Interface/PlayerStateInterface.h"
#include "CCGPlayerState.generated.h"

class ACard3D;
class ACCGMode;
class ACCGPlayerController;

UCLASS()
class CCGPLUGIN_API ACCGPlayerState : public APlayerState
	,public IPlayerStateInterface
{
	GENERATED_BODY()
public:
	friend ACCGMode;
	ACCGPlayerState();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual int32 IncreasePlayerHealth_Implementation(int32 IncreaseBy) override;
	virtual int32 DecreasePlayerHealth_Implementation(int32 DecreaseBy) override;
	virtual int32 IncreasePlayerMana_Implementation(int32 IncreaseBy) override;
	virtual int32 DecreasePlayerMana_Implementation(int32 DecreaseBy) override;
	virtual int32 GetManaForTurn_Implementation() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="디폴트")
	TObjectPtr<ACCGMode> mGameMode;
	UPROPERTY(BlueprintReadWrite, Category="디폴트")
	TObjectPtr<ACCGPlayerController> mOwningPlayerController;
	UPROPERTY(BlueprintReadWrite, Category="디폴트", Replicated)
	int32 mCardGamePlayerId;

public:
	UPROPERTY(BlueprintReadWrite, Category="Attributes", ReplicatedUsing="OnRep_PlayerStat")
	FPlayerStat mPlayerStat;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_PlayerStat();

	void UpdateUI();

	void Init(int32 PlayerID,ACCGPlayerController* PlayerController,ACCGMode* GameMode);
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitAttributes();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayersCardStates(int32 CardsInHand, int32 CardsInDeck, int32 ActiveCards);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetMana(int32 Mana);

	UFUNCTION(Server,Reliable)
	void Server_NotifyPlayerStateChange();

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_ForceUpdateUI();

	FORCEINLINE int32 GetCardGamePlayerId() const { return mCardGamePlayerId; }
};
