// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EffectBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UEffectBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="Get Card Effects for State")
	static UParticleSystem* GetCardEffectForState(ECardEffects Effect, FCard CardStruct, int32 WildcardIndex, USoundCue*& SoundCue);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Get Player Effects for State")
	static UParticleSystem* GetPlayerEffectForState(EBoardPlayerEffects Effect,  USoundCue*& SoundCue);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Spawn Particle and Sound Effects")
	static void SpawnParticleAndSoundEffect(const UObject* WorldContextObject,UParticleSystem* ParticleToSpawn, FVector Location, AActor* ActorForLocation=nullptr, FVector Scale=FVector(1.), bool bAutoDestroy=true, USoundCue* SoundCueToSpawn=nullptr, double Volume=1.);

};
