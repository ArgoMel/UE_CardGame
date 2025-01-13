// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Additional/EditableSliderWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

UEditableSliderWidget::UEditableSliderWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, mBaseValue(0.f)
, mBaseValueMin(0.f)
, mBaseValueMax(1.f)
{
	mText=TEXT("Sample");
}

void UEditableSliderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	EditableSlider->OnValueChanged.AddDynamic(this,&ThisClass::OnSliderValueChanged);
	EditableSliderETB->OnTextCommitted.AddDynamic(this,&ThisClass::OnETBValueChanged);
}

void UEditableSliderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (EditableSliderText)
	{
		EditableSliderText->SetText(FText::FromString(mText));
	}

	const float tempMin=mBaseValueMin;
	const float tempMax=mBaseValueMax;
	mBaseValueMin=FMath::Min(tempMin,tempMax);
	mBaseValueMax=FMath::Max(tempMin,tempMax);
	mBaseValue=FMath::Clamp(mBaseValue,mBaseValueMin,mBaseValueMax);

	if (EditableSlider)
	{
		EditableSlider->SetMinValue(mBaseValueMin);
		EditableSlider->SetMaxValue(mBaseValueMax);
		EditableSlider->SetValue(mBaseValue);
	}
	if (EditableSliderETB)
	{
		EditableSliderETB->SetText(FText::AsNumber(mBaseValue));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UEditableSliderWidget::OnSliderValueChanged(float Value)
{
	OnEditableSliderChanged.Broadcast(Value);
	if (EditableSliderETB)
	{
		EditableSliderETB->SetText(FText::AsNumber(mBaseValue));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UEditableSliderWidget::OnETBValueChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	float value=FCString::Atof(*Text.ToString());
	value=FMath::Clamp(mBaseValueMin,mBaseValueMax,value);
	OnEditableSliderChanged.Broadcast(value);

	if (EditableSlider)
	{
		EditableSlider->SetValue(value);
	}
}
