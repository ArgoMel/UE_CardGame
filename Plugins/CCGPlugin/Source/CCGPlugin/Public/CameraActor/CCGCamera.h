// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CCGCamera.generated.h"

UCLASS()
class CCGPLUGIN_API ACCGCamera : public ACameraActor
{
	GENERATED_BODY()
public:
	ACCGCamera();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
