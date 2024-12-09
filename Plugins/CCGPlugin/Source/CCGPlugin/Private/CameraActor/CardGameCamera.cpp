// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraActor/CardGameCamera.h"

#include "Camera/CameraComponent.h"

ACardGameCamera::ACardGameCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	SetNetUpdateFrequency(1.f);

	GetRootComponent()->bAutoActivate = true;

	GetCameraComponent()->SetConstraintAspectRatio(false);
	GetCameraComponent()->bLockToHmd=false;
	GetCameraComponent()->ComponentTags.Add(FName(TEXT("Player2")));
}
