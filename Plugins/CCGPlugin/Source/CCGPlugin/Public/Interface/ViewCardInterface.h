// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "UObject/Interface.h"
#include "ViewCardInterface.generated.h"

UINTERFACE()
class UViewCardInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IViewCardInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Image")
	void SetCardFrontImage(UMaterialInterface* Material);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Image")
	void SetCardBackImage(UMaterialInterface* Material);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetViewCardData(EViewCard ViewCard,const FText& Text);
};
