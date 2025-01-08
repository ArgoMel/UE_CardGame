// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardLayoutTypeBFL.h"

FWidgetTransform UCardLayoutTypeBFL::Fanned(int32 CardLoopIndex,int32 CardsInHand,FCardLayout Cal)
{
	FWidgetTransform returnValue;
	const float center=(CardsInHand-1)/2.f+Cal.Offset;
	const float tempFloat1=static_cast<float>(CardLoopIndex)-center;
	const float lerp=FMath::Lerp(Cal.SpacingMax,Cal.SpacingMin,CardsInHand/static_cast<float>(Cal.MaxCardsInHand));
	const float clamp=FMath::Clamp(lerp,Cal.SpacingMin,Cal.SpacingMax);
	const float dynamicSpacing=Cal.DynamicSpacing?clamp:Cal.SpacingMin;
	const float tempFloat2=tempFloat1*dynamicSpacing;
	const float tempFloat3=tempFloat1*20.f*Cal.RotationScaleFactor;
	const float flyInDir1=Cal.FlyInDirection?tempFloat2:-tempFloat2;
	const float flyInDir2=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float flyInDir3=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float indexCheck=CardLoopIndex>=center?flyInDir3:flyInDir2;
	const float rotateCardToOffset=Cal.RotateCardsToOffset?indexCheck:0.f;
	
	returnValue.Translation.X=Cal.ScreenOffsetX+flyInDir1*Cal.Scale;
	returnValue.Translation.Y=((tempFloat2*Cal.RadialOffset*tempFloat1*-1.f)-Cal.ScreenOffsetY)*Cal.Scale;
	returnValue.Scale=FVector2D(Cal.Scale);
	returnValue.Angle=-rotateCardToOffset;
	return returnValue;
}

FWidgetTransform UCardLayoutTypeBFL::Horizontal(int32 CardLoopIndex,int32 CardsInHand,FCardLayout Cal)
{
	FWidgetTransform returnValue;
	const float center=(Cal.MaxCardsInHand-1)/2.f+Cal.Offset;
	const float tempFloat1=static_cast<float>(CardLoopIndex)-center;
	const float tempFloat2=tempFloat1*Cal.SpacingMin;
	const float tempFloat3=tempFloat1*20.f*Cal.RotationScaleFactor;
	const float flyInDir1=Cal.FlyInDirection?tempFloat2*Cal.Scale:-tempFloat2*Cal.Scale;
	const float flyInDir2=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float flyInDir3=Cal.FlyInDirection?-tempFloat3:tempFloat3;
	const float indexCheck=CardLoopIndex>=Cal.MaxCardsInHand?flyInDir3:flyInDir2;
	const float rotateCardToOffset=Cal.RotateCardsToOffset?indexCheck:0.f;
	
	returnValue.Translation.X=Cal.ScreenOffsetX+flyInDir1;
	returnValue.Translation.Y=((tempFloat2*Cal.RadialOffset*tempFloat1*-1.f)+1.f-Cal.ScreenOffsetY)*Cal.Scale;
	returnValue.Scale=FVector2D(Cal.Scale);
	returnValue.Shear=FVector2D::ZeroVector;
	returnValue.Angle=-rotateCardToOffset;
	return returnValue;
}
