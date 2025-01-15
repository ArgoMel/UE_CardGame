// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "GameFramework/SaveGame.h"
#include "CCGSG.generated.h"

UCLASS()
class CCGPLUGIN_API UCCGSG : public USaveGame
{
	GENERATED_BODY()
public:
	UCCGSG();
	
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TArray<FString> mDeckList;

	/** 아레나셀렉션에서 컨트롤러를 통해서 저장 */
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category="GameMode Setting")
	FCardGameOption mCardGameOption;

	/** 컨트롤러를 통해서 저장 */
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category="Player Setting")
	TObjectPtr<UMaterialInterface> mProfileMaterial;

public:
	UFUNCTION(BlueprintPure)
	void GetDeckListByName(TArray<FName>& Names) const;
};
