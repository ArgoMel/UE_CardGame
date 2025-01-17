// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCGAIController.h"
#include "Common/CCGStruct.h"
#include "GameFramework/Pawn.h"
#include "Interface/PlayerStateInterface.h"
#include "CCGAIPawn.generated.h"

class ACCGMode;

UCLASS()
class CCGPLUGIN_API ACCGAIPawn : public APawn
	,public IPlayerStateInterface
{
	GENERATED_BODY()
public:
	friend ACCGMode;
	ACCGAIPawn();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIData")
	FString mAIName;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGMode> mGameMode;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<AController> mOwningController;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	int32 mPlayerID;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	int32 mCardGameAiID;

	int32 mPlayerTurn;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIData", ReplicatedUsing="OnRep_AIStat")
	FPlayerStat mAIStat;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_AIStat();
	
public:
	void IncreaseTurn();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UpdateAIState(int32 CardsInHand, int32 CardsInDeck);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitAttributes();

	UFUNCTION(Server, Reliable)
	void Multicast_ForceUIUpdate(int32 CardsInHand, int32 CardsInDeck);
	
	FORCEINLINE FString GetAIName() const{return mAIName;}
	FORCEINLINE int32 GetPlayerId() const { return mPlayerID; }
	
	FORCEINLINE void SetAIName(FString Name){mAIName=Name;}
};
