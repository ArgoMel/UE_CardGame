// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UMathBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="PercentageOf(Float)")
	static float PercentageOf(float Percentage, float Total);

	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="PercentageOf(Int)")
	static float PercentageOfInt32(int32 Percentage, int32 Total);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 WeightedIntInRange(int32 MaxResult);
};
