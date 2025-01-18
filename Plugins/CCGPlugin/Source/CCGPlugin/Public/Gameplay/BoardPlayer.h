// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "GameFramework/Actor.h"
#include "BoardPlayer.generated.h"

class ACCGMode;
class ACameraActor;
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
	friend ACCGMode;
	ABoardPlayer();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UBoxComponent> mBox;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mPlayerTile;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mOutline;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mHeartIcon;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTextRenderComponent> mHealthText;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UAutoRotateToPlayer> mAutoRotateToPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UTimelineComponent> mScaleHealthIcon;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Player")
	TObjectPtr<ACameraActor> mCardGameCamera;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Player", Replicated)
	FText mPlayerName;
	/** The Player Index  */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Player", Replicated)
	int32 mPlayerIndex;
	
	/** Player Health  */
	UPROPERTY(BlueprintReadWrite, Category="Settings", ReplicatedUsing="OnRep_Health")
	int32 mPlayerHealth;
	
	/** 파티클 에셋 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TArray<UParticleSystem*> mParticles;
	/** 사운드 에셋 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TArray<USoundBase*> mSounds;
	/** 커브 에셋 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TObjectPtr<UCurveFloat> mCurve;
	
protected:
	/** Please add a function description */
	UFUNCTION()
	void OnRep_Health();

	UFUNCTION(Category="Timeline")
	void TimelineProgress(float Value);
	UFUNCTION(Category="Bind")
	void TimelineFinished();
	
public:
	UFUNCTION(BlueprintCallable, Category="Multicast")
	void Multicast_BoardPlayerEffect(EBoardPlayerEffects Effect);
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void Server_UpdateHealth(int32 Health);

	FORCEINLINE int32 GetPlayerIndex() const { return mPlayerIndex; }
	FORCEINLINE ACameraActor* GetCardGameCamera() { return mCardGameCamera; }
};
