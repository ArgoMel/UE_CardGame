// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGame/CCGSG.h"

#include "CCGPlugin.h"

UCCGSG::UCCGSG()
{
	GetObjectAsset(mProfileMaterial,UMaterialInterface,"/CCGPlugin/CCGToolkit/CardGame/CardSets/Materials/Card_Mat_Parent.Card_Mat_Parent");
}

void UCCGSG::GetDeckListByName(TArray<FName>& Names) const
{
	Names.Empty();
	Names.Append(mDeckList);
}
