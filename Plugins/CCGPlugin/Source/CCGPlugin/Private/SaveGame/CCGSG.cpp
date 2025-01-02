// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGame/CCGSG.h"

void UCCGSG::GetDeckListByName(TArray<FName>& Names) const
{
	Names.Empty();
	Names.Append(mDeckList);
}
