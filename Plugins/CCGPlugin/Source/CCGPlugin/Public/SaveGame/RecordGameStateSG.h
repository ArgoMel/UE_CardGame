// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "GameFramework/SaveGame.h"
#include "RecordGameStateSG.generated.h"

UCLASS()
class CCGPLUGIN_API URecordGameStateSG : public USaveGame
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TArray<FGameStateSnapshot> mSnapshot;

};
