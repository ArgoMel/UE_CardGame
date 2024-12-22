// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraActor/CCGCamera.h"

#include "Camera/CameraComponent.h"

ACCGCamera::ACCGCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	SetNetUpdateFrequency(1.f);
	//AutoActivateForPlayer=EAutoReceiveInput::Type::Player0;
	
	GetCameraComponent()->SetConstraintAspectRatio(false);
	GetCameraComponent()->PostProcessSettings.AutoExposureBias=0.f;
	GetCameraComponent()->PostProcessSettings.AutoExposureLowPercent=1.f;
	GetCameraComponent()->PostProcessSettings.AutoExposureHighPercent=1.f;
}

void ACCGCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

