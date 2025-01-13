// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditableSliderWidget.generated.h"

class UEditableTextBox;
class USlider;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditableSliderChanged, float, Value);

UCLASS(Abstract)
class CCGPLUGIN_API UEditableSliderWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	explicit UEditableSliderWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> EditableSliderText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> EditableSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableSliderETB;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString mText;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float mBaseValue;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float mBaseValueMin;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float mBaseValueMax;

public:
	UPROPERTY(BlueprintAssignable, Category="Delegate")
	FOnEditableSliderChanged OnEditableSliderChanged;

protected:
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	UFUNCTION()
	void OnETBValueChanged(const FText& Text, ETextCommit::Type CommitMethod);
};
