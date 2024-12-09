// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CCGBFL.h"

#include "Kismet/KismetSystemLibrary.h"

bool UCCGBFL::CanExecuteCosmeticEvents(const UObject* WorldContextObject)
{
	return !UKismetSystemLibrary::IsDedicatedServer(WorldContextObject);
}
