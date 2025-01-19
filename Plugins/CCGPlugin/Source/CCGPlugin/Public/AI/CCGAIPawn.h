// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCGAIController.h"
#include "Common/CCGStruct.h"
#include "GameFramework/Pawn.h"
#include "Interface/PlayerStateInterface.h"
#include "CCGAIPawn.generated.h"

class ACCGPlayerController;
class ACCGMode;
class ACCGAIController;

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
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual int32 IncreasePlayerHealth_Implementation(int32 IncreaseBy) override;
	virtual int32 DecreasePlayerHealth_Implementation(int32 DecreaseBy) override;
	virtual int32 IncreasePlayerMana_Implementation(int32 IncreaseBy) override;
	virtual int32 DecreasePlayerMana_Implementation(int32 DecreaseBy) override;
	virtual int32 GetManaForTurn_Implementation() override;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="AIData")
	FString mAIName;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGMode> mGameMode;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGAIController> mOwningController;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGPlayerController> mPlayerController;
	/** 게임모드에서 설정 */
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	int32 mPlayerID;

public:
	UPROPERTY(BlueprintReadWrite, Category="AIData", ReplicatedUsing="OnRep_AIStat")
	FPlayerStat mAIStat;

protected:
	/** Please add a function description */
	UFUNCTION()
	void OnRep_AIStat();

	void UpdateUI();
	
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
