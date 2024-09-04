// Fill out your copyright notice in the Description page of Project Settings.

#include "CardGameCamera.h"
#include "Camera/CameraComponent.h"

ACardGameCamera::ACardGameCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	NetUpdateFrequency = 1.f;

	GetRootComponent()->bAutoActivate = true;

	GetCameraComponent()->SetConstraintAspectRatio(false);
	GetCameraComponent()->bLockToHmd=false;
	GetCameraComponent()->ComponentTags.Add(FName(TEXT("Player2")));
}
