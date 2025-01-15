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
	TObjectPtr<UStaticMeshComponent> ToLocation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> Target;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Component")
	TObjectPtr<UStaticMeshComponent> NarrowCapsule;

public:
	UFUNCTION(Category="Event")
	void SetSelectionProperties(FVector Loc,bool ValidTarget) const;
};
