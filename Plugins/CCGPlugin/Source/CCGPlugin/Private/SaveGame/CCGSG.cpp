// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGame/CCGSG.h"

#include "CCGPlugin.h"

UCCGSG::UCCGSG()
{
	GetObjectAsset(mProfileTexture,UTexture2D,"/CCGPlugin/CCGToolkit/CardGame/CardSets/Textures/Character_1.Character_1");
}

void UCCGSG::GetDeckListByName(TArray<FName>& Names) const
{
	Names.Empty();
	Names.Append(mDeckList);
}
