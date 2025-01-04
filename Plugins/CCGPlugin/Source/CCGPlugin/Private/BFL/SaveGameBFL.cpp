// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/SaveGameBFL.h"

#include "CCGPlugin.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/CCGSG.h"
#include "SaveGame/CustomDeckSG.h"

void USaveGameBFL::RemoveSaveGameSlot(FString SaveGame)
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveGame,0))
	{
		return;
	}

	UCCGSG* loadData=Cast<UCCGSG>(UGameplayStatics::LoadGameFromSlot(CCG_SaveSlotName::CardGameSave,0));
	IF_RET_VOID(loadData);
	loadData->mDeckList.Remove(SaveGame);
	UGameplayStatics::SaveGameToSlot(loadData,CCG_SaveSlotName::CardGameSave,0);
	UGameplayStatics::DeleteGameInSlot(SaveGame,0);
}

bool USaveGameBFL::CreateOrLoadCardGameSave(FString SlotName, TSubclassOf<USaveGame> SaveGameClass, USaveGame*& SaveGame)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		SaveGame=UGameplayStatics::LoadGameFromSlot(SlotName,0);
		return true;
	}
	SaveGame=UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	return UGameplayStatics::SaveGameToSlot(SaveGame,SlotName,0);
}

bool USaveGameBFL::SaveCustomDeck(const FText& InText, TArray<FName>& CustomDeck, bool bEditable)
{
	if (!UGameplayStatics::DoesSaveGameExist(CCG_SaveSlotName::CardGameSave,0))
	{
		USaveGame* saveGame;
		CreateOrLoadCardGameSave(CCG_SaveSlotName::CardGameSave,UCCGSG::StaticClass(),saveGame);
	}
	UCCGSG* loadData=Cast<UCCGSG>(UGameplayStatics::LoadGameFromSlot(CCG_SaveSlotName::CardGameSave,0));
	if (!loadData)
	{
		return false;
	}

	const FString customDeckName=InText.ToString();
	if (!loadData->mDeckList.Find(customDeckName))
	{
		loadData->mDeckList.Add(customDeckName);
		UGameplayStatics::SaveGameToSlot(loadData,CCG_SaveSlotName::CardGameSave,0);
		USaveGame* saveGame;
		CreateOrLoadCardGameSave(customDeckName,UCustomDeckSG::StaticClass(),saveGame);
	}
	UCustomDeckSG* customDeckData=Cast<UCustomDeckSG>(UGameplayStatics::LoadGameFromSlot(customDeckName,0));
	if (!customDeckData)
	{
		return false;
	}

	customDeckData->mCustomDeck=CustomDeck;
	customDeckData->bEditable=bEditable;
	return UGameplayStatics::SaveGameToSlot(customDeckData,customDeckName,0);
}

bool USaveGameBFL::LoadCustomDeck(FString DeckName, TArray<FName>& CustomDeck, bool& bDeckEditable)
{
	if (!UGameplayStatics::DoesSaveGameExist(DeckName,0))
	{
		UE_LOG(LogTemp, Warning, TEXT("슬롯: %s 존재하지 않음"),*DeckName);
		bDeckEditable=false;
		return false;
	}

	const UCustomDeckSG* loadData=Cast<UCustomDeckSG>(UGameplayStatics::LoadGameFromSlot(DeckName,0));
	if (!loadData)
	{
		UE_LOG(LogTemp, Warning, TEXT("슬롯: %s 존재하지 않음"),*DeckName);
		bDeckEditable=false;
		return false;
	}

	CustomDeck=loadData->mCustomDeck;
	bDeckEditable=loadData->bEditable;
	return true;
}

bool USaveGameBFL::GetRandomDeck(FString& RandomDeck)
{
	if (!UGameplayStatics::DoesSaveGameExist(CCG_SaveSlotName::CardGameSave,0))
	{
		UE_LOG(LogTemp, Warning, TEXT("슬롯: %s 존재하지 않음"),*CCG_SaveSlotName::CardGameSave);
		return false;
	}

	const UCCGSG* loadData=Cast<UCCGSG>(UGameplayStatics::LoadGameFromSlot(CCG_SaveSlotName::CardGameSave,0));
	IF_RET_BOOL(loadData);
	const int32 randIndex=FMath::RandRange(0,loadData->mDeckList.Num()-1);
	RandomDeck=loadData->mDeckList[randIndex];
	return true;
}

bool USaveGameBFL::GetDeckValid(FString SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName,0);
}

bool USaveGameBFL::GetDeckEditable(FString SlotName)
{
	const UCustomDeckSG* loadData=Cast<UCustomDeckSG>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
	IF_RET_BOOL(loadData);
	return loadData->bEditable;
}
