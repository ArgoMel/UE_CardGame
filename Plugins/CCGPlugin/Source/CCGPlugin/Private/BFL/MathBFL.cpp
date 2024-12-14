// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/MathBFL.h"

float UMathBFL::PercentageOf(double Percentage, double Total)
{
	return Percentage / (Percentage+Total);
}

int32 UMathBFL::PercentageOfInt32(int32 Percentage, int32 Total)
{
	if (Percentage<=0)
	{
		return 0;
	}
	return PercentageOf(Percentage,Total);
}

int32 UMathBFL::WeightedIntInRange(int32 MaxResult)
{
	return MaxResult;
}
