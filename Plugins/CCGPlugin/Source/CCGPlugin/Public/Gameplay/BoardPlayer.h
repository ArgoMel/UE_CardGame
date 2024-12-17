// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardPlayer.generated.h"

class ACCGCamera;
class UTimelineComponent;
class UBoxComponent;
class UArrowComponent;
class UTextRenderComponent;
class UAutoRotateToPlayer;

UCLASS()
class CCGPLUGIN_API ABoardPlayer : public AActor
{
	GENERATED_BODY()
public:
	ABoardPlayer();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mPlayerTile;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UAutoRotateToPlayer> mAutoRotateToPlayer;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mOutline;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mHealthText;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mHeartIcon;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UArrowComponent> mArrow;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<USceneComponent> mScene;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UBillboardComponent> mBillboard;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UTextRenderComponent> mPlayer;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UBoxComponent> mBox;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="디폴트")
	TObjectPtr<UStaticMeshComponent> mCube;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TObjectPtr<UMaterialInstanceDynamic> mMaterialInstance;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="BoardPlayer")
	TObjectPtr<UTimelineComponent> mScaleHealthIcon;

	/** Player Health  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settings", ReplicatedUsing="OnRep_Health")
	int32 mPlayerHealth;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settings")
	FVector tHealthIconRelativeScale;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settings")
	bool bScalingHealthIcon;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settings")
	FLinearColor tHealthTextColour;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FLinearColor tDecreaseColour;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	FLinearColor mHealthIncreaseColour;

public:
	/** The Player Index  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player", Replicated)
	int32 mPlayerIndex;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player", Replicated)
	FText mPlayerName;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Player")
	TObjectPtr<ACCGCamera> mCardGameCamera;

	/** Owning Contoller */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Settings")
	TObjectPtr<AController> mOwningController;

public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_Health();
};
