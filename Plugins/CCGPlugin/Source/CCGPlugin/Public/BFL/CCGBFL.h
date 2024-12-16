// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CCGBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UCCGBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool CanExecuteCosmeticEvents(const UObject* WorldContextObject);

	template <class T>
	static void ShuffleArray(T& InArray, int32 Seed)
	{
		const FRandomStream Stream{Seed};
		const int32 LastIndex = InArray.Num() - 1;
		for (int32 i = 0; i < LastIndex; ++i)
		{
			int32 Index = Stream.RandRange(0, LastIndex);
			if (i != Index)
			{
				InArray.Swap(i, Index);
			}
		}
	}
};
