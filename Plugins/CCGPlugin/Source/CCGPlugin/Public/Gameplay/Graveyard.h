// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card3D.h"
#include "GameFramework/Actor.h"
#include "Graveyard.generated.h"

class UAutoRotateToPlayer;

UCLASS()
class CCGPLUGIN_API AGraveyard : public AActor
{
	GENERATED_BODY()
public:
	AGraveyard();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UBoxComponent> mCollisionBox;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mGraveyardText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mGraveyardMesh;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mOutline;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UAutoRotateToPlayer> mPlayerRotation;

	/** An Array which holds all 3D Cards which have been sent to the graveyard. */
	UPROPERTY(BlueprintReadWrite, Category="Graveyard", Replicated)
	TArray<ACard3D*> mActorsInGraveyard;
	/** Which player index this graveyard belongs to.  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Graveyard")
	int32 mPlayerIndex;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Graveyard", Replicated)
	TArray<FName> mGraveyardList;
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,Category="Graveyard")
	void ScaleCardGraveyardStaticMesh();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,Category="Graveyard")
	void HideUnseenCardsInGraveyard();
	/** Please add a function description */
	UFUNCTION(BlueprintPure,Category="Graveyard")
	FVector GetCardInGraveyardLoc(int32 Add=0) const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,Category="Graveyard")
	void RecreateTopGraveyardCardActor();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,Category="Graveyard")
	void RemoveCardFromGraveyard(FName CardName, int32 Index);

	UFUNCTION(NetMulticast,Unreliable)
	void Multicast_AddToGraveyardVisual();
	
	UFUNCTION(Server,Reliable)
	void Server_AddToGraveyard(ACard3D* Card,FName Name=FName());
	UFUNCTION(Server,Reliable)
	void Server_RemoveCardFromGraveyard(FName Card, int32 Index);

	FORCEINLINE int32 GetPlayerIndex() const { return mPlayerIndex; }
};
