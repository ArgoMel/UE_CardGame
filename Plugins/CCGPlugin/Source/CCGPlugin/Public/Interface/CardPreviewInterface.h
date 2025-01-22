// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CardPreviewInterface.generated.h"

class ACard3D;
enum class ECardSet : uint8;

UINTERFACE()
class UCardPreviewInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API ICardPreviewInterface
{
	GENERATED_BODY()
public:
	/** Card3d에서 호출 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitiateCardPreview(FName CardName,ECardSet CardSet, ACard3D* CallingParent, float Scale);
};
