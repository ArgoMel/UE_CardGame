// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/MiscBFL.h"

#include "CCGPlugin.h"
#include "BFL/CCGBFL.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraActor.h"
#include "GameInstance/CCGameInstance.h"
#include "Gameplay/Card3D.h"
#include "Interface/WidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerController/CCGPlayerController.h"
#include "PlayerState/CCGPlayerState.h"

TSubclassOf<UUserWidget> UMiscBFL::mDisplayWidgetClass;

UMiscBFL::UMiscBFL()
{
	GetClassAsset(mDisplayWidgetClass,UUserWidget,"/CCGPlugin/CCGToolkit/BP/Widgets/Message/WBP_DisplayMessage.WBP_DisplayMessage_C")
}

bool UMiscBFL::InterpToGoalLocation2D(FWidgetTransform CurrentTransform, FWidgetTransform GoalTransform, double DeltaTime, double InterpSpeed, FWidgetTransform& Transform)
{
	Transform.Translation=UKismetMathLibrary::Vector2DInterpTo(CurrentTransform.Translation, GoalTransform.Translation, DeltaTime, InterpSpeed);
	Transform.Scale=UKismetMathLibrary::Vector2DInterpTo(CurrentTransform.Scale, GoalTransform.Scale, DeltaTime, InterpSpeed);
	Transform.Angle=UKismetMathLibrary::FInterpTo(CurrentTransform.Angle, GoalTransform.Angle, DeltaTime, InterpSpeed);

	const bool isArrived=FMath::IsNearlyEqual(Transform.Translation.Size(),GoalTransform.Translation.Size())&&
		FMath::IsNearlyEqual(Transform.Scale.Size(),GoalTransform.Scale.Size())&&
			FMath::IsNearlyEqual(Transform.Angle,GoalTransform.Angle);
	if (isArrived)
	{
		Transform=GoalTransform;
	}
	return isArrived;
}

void UMiscBFL::MouseToWorldLocation(APlayerController* PlayerController, FHitResult& OutHit)
{
	IF_RET_VOID(PlayerController);
	const UWorld* world=PlayerController->GetWorld();
	IF_RET_VOID(world);
	const UCCGameInstance* gameInstance=Cast<UCCGameInstance>(world->GetGameInstance());
	IF_RET_VOID(gameInstance);
	bool isMobile=gameInstance->IsMobilePlatform();
	FVector worldLoc;
	FVector worldDir;
	
	if (isMobile)
	{
		float locX;
		float locY;
		PlayerController->GetInputTouchState(ETouchIndex::Type::Touch1,locX,locY,isMobile);

		UGameplayStatics::DeprojectScreenToWorld(PlayerController,FVector2D(locX,locY),worldLoc,worldDir);
	}
	else
	{
		PlayerController->DeprojectMousePositionToWorld(worldLoc,worldDir);
	}
	FCollisionObjectQueryParams	objectParams;
	objectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	objectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	const FCollisionQueryParams collisionParams;
	world->LineTraceSingleByObjectType(OutHit,worldLoc,worldLoc*100000.f,objectParams,collisionParams);
}

int32 UMiscBFL::ReadInteractingCardDataAttack(ACard3D* ReadCard)
{
	if (!ReadCard)
	{
		return 0;
	}
	return ReadCard->GetAttack();
}

FTransform UMiscBFL::MouseDistanceInWorldSpace(ACCGPlayerController* PlayerController, double Distance)
{
	IF_RET(FTransform(),PlayerController);
	const UWorld* world=PlayerController->GetWorld();
	IF_RET(FTransform(),world);
	FTransform spawnTransform;
	FVector worldLoc;
	FVector worldDir;
	PlayerController->DeprojectMousePositionToWorld(worldLoc,worldDir);
	spawnTransform.SetLocation(worldLoc+worldDir*Distance);

	const FRotator worldRot=GetWorldRotationForPlayer(world,FRotator::ZeroRotator);
	spawnTransform.SetRotation(worldRot.Quaternion());
	spawnTransform.SetScale3D(FVector(1.f));
	return spawnTransform;
}

void UMiscBFL::CreateScreenDisplayMessage(const UObject* WorldContextObject, const FString& Message, FLinearColor SpecifiedColor, double Duration)
{
	IF_RET_VOID(WorldContextObject);
	const UWorld* world=WorldContextObject->GetWorld();
	IF_RET_VOID(world);
	if (!UCCGBFL::CanExecuteCosmeticEvents(world))
	{
		return;
	}
	UUserWidget* displayMsg=CreateWidget<UUserWidget>(world->GetFirstPlayerController(),mDisplayWidgetClass);
	IF_RET_VOID(displayMsg);
	if (displayMsg->Implements<UWidgetInterface>())
	{
		IWidgetInterface::Execute_DisplayMessage(displayMsg,Message,SpecifiedColor,Duration);
	}
	displayMsg->AddToViewport(CCG_ZOrder::DisplayMessage);
}

void UMiscBFL::AddMsgToLog(ACCGPlayerController* CallingPlayer, const FString& Message, FLinearColor SpecifiedColor, double Duration)
{
	IF_RET_VOID(CallingPlayer);
	UUserWidget* playerUI=CallingPlayer->GetPlayerGameUI();
	IF_RET_VOID(playerUI);
	if (playerUI->Implements<UWidgetInterface>())
	{
		IWidgetInterface::Execute_DisplayMessage(playerUI,Message,SpecifiedColor,Duration);
	}
}

FRotator UMiscBFL::GetWorldRotationForPlayer(const UObject* WorldContextObject, FRotator OverrideRotationAxis)
{
	FRotator returnRot=FRotator::ZeroRotator;
	IF_RET(returnRot,WorldContextObject);
	const UWorld* world=WorldContextObject->GetWorld();
	IF_RET(returnRot,world);
	IF_RET(returnRot,world->GetFirstPlayerController());
	const APlayerCameraManager* cameraManager=world->GetFirstPlayerController()->PlayerCameraManager;
	IF_RET(returnRot,cameraManager);

	if (OverrideRotationAxis.Roll>0.f)
	{
		returnRot.Roll=OverrideRotationAxis.Roll;
	}
	if (OverrideRotationAxis.Pitch>0.f)
	{
		returnRot.Pitch=OverrideRotationAxis.Pitch;
	}
	if (OverrideRotationAxis.Yaw>0.f)
	{
		returnRot.Yaw=OverrideRotationAxis.Yaw;
	}
	else
	{
		returnRot.Yaw=cameraManager->GetCameraRotation().Yaw;
	}
	return returnRot;
}

FVector2D UMiscBFL::GetMousePositionInRange(UWorld* World, FVector2D SizeOffset, double GlobalEdgeOffset, FVector2D EdgeOffset)
{
	IF_RET(FVector2D::ZeroVector,World);
	const APlayerController* playerController=World->GetFirstPlayerController();
	IF_RET(FVector2D::ZeroVector,playerController);

	FVector2D returnValue;
	float locX;
	float locY;
	playerController->GetMousePosition(locX,locY);
	const FVector2D viewportSize=UWidgetLayoutLibrary::GetViewportSize(World);
	returnValue.X=FMath::Clamp(locX,GlobalEdgeOffset,
		viewportSize.X-GlobalEdgeOffset-(SizeOffset.X+EdgeOffset.X));
	returnValue.Y=FMath::Clamp(locY,GlobalEdgeOffset+EdgeOffset.Y,
		viewportSize.Y-GlobalEdgeOffset-(SizeOffset.Y-EdgeOffset.Y))-EdgeOffset.Y;
	return returnValue;
}

FVector2D UMiscBFL::GetPositionInRange(UObject* WorldContextObject, double GlobalEdgeOffset, FVector2D EdgeOffset)
{
	const FVector2D viewportSize=UWidgetLayoutLibrary::GetViewportSize(WorldContextObject);
	const FVector2D randSize(FMath::RandRange(0.,viewportSize.X),FMath::RandRange(0.,viewportSize.Y));
	FVector2D returnValue;
	returnValue.X=FMath::Clamp(randSize.X,GlobalEdgeOffset,viewportSize.X-GlobalEdgeOffset-EdgeOffset.X);
	returnValue.Y=FMath::Clamp(randSize.Y,GlobalEdgeOffset+EdgeOffset.Y,viewportSize.Y-GlobalEdgeOffset-EdgeOffset.Y)-EdgeOffset.Y;
	return returnValue;
}

float UMiscBFL::ModifyDPIScaling(UObject* WorldContextObject, double Value, bool InvertDPIScaling)
{
	const float viewportScale=UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);
	const float invertValue=2.f-viewportScale;
	if (InvertDPIScaling)
	{
		return invertValue*Value;
	}
	return Value/invertValue;
}

ACameraActor* UMiscBFL::GetCardGamePlayerCamera(ACCGPlayerController* PlayerController)
{
	IF_RET_NULL(PlayerController);
	ABoardPlayer* boardPlayer= PlayerController->GetBoardPlayer();
	IF_RET_NULL(boardPlayer);
	return boardPlayer->GetCardGameCamera();
}

float UMiscBFL::CalculateFloatPrecision(double Float)
{
	const int32 tempInt=static_cast<int32>(Float * 10);
	return tempInt*0.1f;
}

float UMiscBFL::GetWaitTimeWithRandomDeviation(double WaitTime, double RandomDeviation)
{
	return  WaitTime+FMath::RandRange(-RandomDeviation,RandomDeviation);
}

FTransform UMiscBFL::ScreenPositionInWorldSpace(ACCGPlayerController* PlayerController,
	FVector2D ScreenPosition, double ForwardDistance)
{
	IF_RET(FTransform(),PlayerController);
	const UWorld* world=PlayerController->GetWorld();
	IF_RET(FTransform(),world);
	FTransform spawnTransform;
	const float viewportScale=UWidgetLayoutLibrary::GetViewportScale(PlayerController);
	FVector worldLoc;
	FVector worldDir;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController,ScreenPosition*viewportScale,worldLoc,worldDir);
	const FRotator worldRot= GetWorldRotationForPlayer(world,FRotator::ZeroRotator);

	spawnTransform.SetLocation(worldLoc+worldDir*ForwardDistance);
	spawnTransform.SetRotation(worldRot.Quaternion());
	spawnTransform.SetScale3D(FVector(1.f));
	return spawnTransform;
}
