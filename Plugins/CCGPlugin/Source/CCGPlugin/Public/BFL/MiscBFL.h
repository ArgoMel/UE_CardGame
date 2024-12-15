// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Slate/WidgetTransform.h"
#include "MiscBFL.generated.h"

class ACCGCamera;
class ACCGPlayerController;
class ACard3D;

UCLASS()
class CCGPLUGIN_API UMiscBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UMiscBFL();

private:
	static TSubclassOf<UUserWidget> mDisplayWidgetClass;
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool InterpToGoalLocation2D(FWidgetTransform CurrentTransform, FWidgetTransform GoalTransform, double DeltaTime, double InterpSpeed, FWidgetTransform& Transform);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Default")
	static void MouseToWorldLocation(APlayerController* PlayerController, FHitResult& OutHit);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 ReadInteractingCardDataAttack(ACard3D* ReadCard);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void MouseDistanceInWorldSpace(const UWorld* World,ACCGPlayerController* PlayerController, double Distance, FTransform& SpawnTransform);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void CreateScreenDisplayMessage(const UWorld* World,const FString& Message, FLinearColor SpecifiedColor, double Duration);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="GetPlayerStateReference")
	static int32 GetPlayerID(AController* Controller);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool GetWorldRotationForPlayer(const UWorld* World,FRotator OverrideRotationAxis, FRotator& ReturnRotation);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static FVector2D GetMousePositionInRange(UWorld* World,FVector2D SizeOffset, double GlobalEdgeOffset, FVector2D EdgeOffset);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static FVector2D GetPositionInRange(UObject* WorldContextObject,double GlobalEdgeOffset, FVector2D EdgeOffset);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static float ModifyDPIScaling(UObject* WorldContextObject,double Value, bool InvertDPIScaling);
	
	/** 플레이어 스테이트가 들고있는게 나은듯 */
	UFUNCTION(BlueprintCallable)
	static ACCGCamera* GetCardGamePlayerCamera(ACCGPlayerController* PlayerController);
	
	/** 2.213= 2.2 */
	UFUNCTION(BlueprintPure)
	static float CalculateFloatPrecision(double Float);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static float GetWaitTimeWithRandomDeviation(double WaitTime, double RandomDeviation);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void ScreenPositionInWorldSpace(ACCGPlayerController* PlayerController, FVector2D ScreenPosition, double ForwardDistance, FTransform& SpawnTransform);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void AddMessageToLog(ACCGPlayerController* CallingPlayer,const FString& Message, FLinearColor SpecifiedColor, double Duration=5.);
};
