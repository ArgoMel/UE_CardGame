// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/MathBFL.h"

const TArray<float> RandomWeight={0.f,65.f,80.f,90.f,100.f};

float UMathBFL::PercentageOf(float Percentage, float Total)
{
	return Percentage / (Percentage+Total);
}

float UMathBFL::PercentageOfInt32(int32 Percentage, int32 Total)
{
	if (Percentage<=0)
	{
		return 0;
	}
	return PercentageOf(Percentage,Total);
}

int32 UMathBFL::WeightedIntInRange(int32 MaxResult)
{
	const float randNum=FMath::RandRange(0.f,100.f);
	for (int32 i=0;i<RandomWeight.Num()-1;++i)
	{
		if (RandomWeight[i]<=randNum&&
			randNum<=RandomWeight[i+1])
		{
			return FMath::Clamp(i,0,MaxResult);
		}
	}
	return 0;
}
