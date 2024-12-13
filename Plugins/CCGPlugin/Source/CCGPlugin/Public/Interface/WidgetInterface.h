// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

class UWidget;

UINTERFACE()
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IWidgetInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnOption(ECardRowOptions Option,const FString& CardSet, UWidget* CallingWidget);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnSearch(const FText& SearchText);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnCardSet(ECardSet CardSet);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnPreview(bool PreviewEnabled);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnServerSelection();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReturnDeckSelection(const FString& DeckSelection);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisplayMessage(const FString& Message, FLinearColor SpecifiedColor, double Duration);
};
