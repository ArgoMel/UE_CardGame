// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/EffectBFL.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UParticleSystem* UEffectBFL::GetCardEffectForState(ECardEffects Effect, FCard CardStruct, int32 WildcardIndex, USoundCue*& SoundCue)
{
	switch (Effect)
	{
	case ECardEffects::OnCardPlaced:
		SoundCue=CardStruct.PlacementSetting.SoundCue;
		return CardStruct.PlacementSetting.Particle;
	case ECardEffects::OnAttack:
		SoundCue=CardStruct.Attack.SoundCue;
		return CardStruct.Attack.Particle;
	case ECardEffects::OnAbilityUsed:
		SoundCue=CardStruct.Abilities[WildcardIndex].SoundCue;
		return CardStruct.Abilities[WildcardIndex].Particle;
	case ECardEffects::OnDeath:
		SoundCue=CardStruct.Health.DeathSoundCue;
		return CardStruct.Health.DeathParticle;
	default: ;
	}
	SoundCue=nullptr;
	return nullptr;
}

void UEffectBFL::SpawnParticleAndSoundEffect(const UObject* WorldContextObject, UParticleSystem* ParticleToSpawn, FVector Location, AActor* ActorForLocation, FVector Scale, bool bAutoDestroy, USoundBase* SoundCueToSpawn, double Volume)
{
	if (ActorForLocation)
	{
		Location=ActorForLocation->GetActorLocation();
	}
	if (ParticleToSpawn)
	{
		UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, ParticleToSpawn, Location,FRotator::ZeroRotator,Scale,bAutoDestroy);
	}
	if (SoundCueToSpawn)
	{
		UGameplayStatics::SpawnSound2D(WorldContextObject, SoundCueToSpawn, Volume);
	}
}
