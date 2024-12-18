// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardLayoutTypeBFL.h"

FWidgetTransform UCardLayoutTypeBFL::Fanned(FCardLayoutCalculate Cal)
{
	FWidgetTransform returnValue;
	const float center=(Cal.CardsInHand-1)/2.f+Cal.CardOffset;
	const float tempFloat1=static_cast<float>(Cal.CardLoopIndex)-center;
	const float lerp=FMath::Lerp(Cal.CardSpacing.Y,Cal.CardSpacing.X,Cal.CardsInHand/static_cast<float>(Cal.MaxCardInHand));
	const float clamp=FMath::Clamp(lerp,Cal.CardSpacing.X,Cal.CardSpacing.Y);
	const float dynamicSpacing=Cal.DynamicSpacing?clamp:Cal.CardSpacing.X;
	const float tempFloat2=tempFloat1*dynamicSpacing;
	const float tempFloat3=tempFloat1*20.f*Cal.RotationDistanceScaleFactor;
	const float flyInDir1=Cal.FlyInDirection?tempFloat2:-tempFloat2;
	const float flyInDir2=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float flyInDir3=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float indexCheck=Cal.CardLoopIndex>=center?flyInDir3:flyInDir2;
	const float rotateCardToOffset=Cal.RotateCardsToOffset?indexCheck:0.f;
	
	returnValue.Translation.X=Cal.ScreenOffset.X+flyInDir1*Cal.CardScale;
	returnValue.Translation.Y=((tempFloat2*Cal.RadiusOffset*tempFloat1*-1.f)-Cal.ScreenOffset.Y)*Cal.CardScale;
	returnValue.Scale=FVector2D(Cal.CardScale);
	returnValue.Angle=-rotateCardToOffset;
	return returnValue;
}

FWidgetTransform UCardLayoutTypeBFL::Horizontal(FCardLayoutCalculate Cal)
{
	FWidgetTransform returnValue;
	const float center=(Cal.MaxCardInHand-1)/2.f+Cal.CardOffset;
	const float tempFloat1=static_cast<float>(Cal.CardLoopIndex)-center;
	const float tempFloat2=tempFloat1*Cal.CardSpacing.X;
	const float tempFloat3=tempFloat1*20.f*Cal.RotationDistanceScaleFactor;
	const float flyInDir1=Cal.FlyInDirection?tempFloat2*Cal.CardScale:-tempFloat2*Cal.CardScale;
	const float flyInDir2=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float flyInDir3=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float indexCheck=Cal.CardLoopIndex>=Cal.MaxCardInHand?flyInDir3:flyInDir2;
	const float rotateCardToOffset=Cal.RotateCardsToOffset?indexCheck:0.f;
	
	returnValue.Translation.X=Cal.ScreenOffset.X+flyInDir1;
	returnValue.Translation.Y=((tempFloat2*Cal.RadiusOffset*tempFloat1*-1.f)+1.f-Cal.ScreenOffset.Y)*Cal.CardScale;
	returnValue.Scale=FVector2D(Cal.CardScale);
	returnValue.Shear=FVector2D::ZeroVector;
	returnValue.Angle=-rotateCardToOffset;
	return returnValue;
}
