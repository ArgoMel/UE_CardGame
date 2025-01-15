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

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mToLocation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mTarget;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> mNarrowCapsule;

public:
	UFUNCTION(BlueprintCallable,Category="Event")
	void SetSelectionProperties(FVector Location,bool ValidTarget) const;
};
