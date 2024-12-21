// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Card3D.h"
#include "GameFramework/Actor.h"
#include "Graveyard.generated.h"

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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mOutline;

	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
// 	TObjectPtr<UAutoRotateToPlayer_C> PlayerRotationActor;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
// 	TObjectPtr<UArrowComponent> CardPlacementPoint;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
// 	TObjectPtr<UStaticMeshComponent> SM_GraveyardMesh;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
// 	TObjectPtr<UTextRenderComponent> Graveyard_Text;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
// 	TObjectPtr<UBoxComponent> CollisionBox;
	
// 	/** Scene decal Mat Instance.  */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
// 	TObjectPtr<UMaterialInstanceDynamic> OutlineDecal_MaterialInstance;
//
// 	/** Which player index this graveyard belongs to.  */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="디폴트")
// 	int32 Player_Index;
//
// 	/** An Array which holds all 3D Cards which have been sent to the graveyard. */
// 	static_assert(false, "You will need to add DOREPLIFETIME(AGraveyard, ActorsInGraveyard) to GetLifetimeReplicatedProps");
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
// 	TArray<A3DCard_C*> ActorsInGraveyard;

// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
// 	int32 RemoveIndex;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트", Replicated)
	TArray<FName> mGraveyardList;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TArray<FCard> mGraveyardStructList;
	
public:
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable)
// 	void ScaleCardGraveyardStaticMesh();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable)
// 	void HideUnseenCardsInGraveyard();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	FVector GetCardInGraveyardLoc(int32 Add=0);

// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable)
// 	void RecreateTopGraveyardCardActor();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable)
// 	void RemoveCardFromGraveyard(FName CardName, int32 Index);

	UFUNCTION(Server,Reliable)
	void Server_AddToGraveyard(ACard3D* Card,FName Name=FName());

	UFUNCTION(Server,Reliable)
	void Server_RemoveCardFromGraveyard(FName Card, int32 Index);
};
