// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetDragSelection.generated.h"

UCLASS()
class CCGPLUGIN_API ATargetDragSelection : public AActor
{
	GENERATED_BODY()
public:
	ATargetDragSelection();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(Category="Event")
	void SetSelectionProperties(FVector Loc,bool ValidTarget);
};
