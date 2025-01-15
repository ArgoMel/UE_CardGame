// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/BoardPlayer.h"

#include "CCGPlugin.h"
#include "BFL/CCGBFL.h"
#include "BFL/EffectBFL.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/TimelineComponent.h"
#include "Gameplay/ActorComponent/AutoRotateToPlayer.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/CCGPlayerController.h"

// Sets default values
ABoardPlayer::ABoardPlayer()
: mPlayerIndex(0)
, mPlayerHealth(0)
{
	PrimaryActorTick.bCanEverTick = true;
	
	mParticles.Init(nullptr,static_cast<int32>(EBoardPlayerEffects::Max));
	mSounds.Init(nullptr,static_cast<int32>(EBoardPlayerEffects::Max));
	GetObjectAsset(mCurve,UCurveFloat,"/CCGPlugin/CCGToolkit/Curve/C_ScaleHealthIcon.C_ScaleHealthIcon");

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	mBillboard=CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	mBillboard->SetupAttachment(RootComponent);

	mPlayer=CreateDefaultSubobject<UTextRenderComponent>(TEXT("Player"));
	mPlayer->SetupAttachment(RootComponent);

	mBox=CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mBox->SetupAttachment(RootComponent);

	mCube=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	mCube->SetupAttachment(RootComponent);

	mPlayerTile=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerTile"));
	mPlayerTile->SetupAttachment(RootComponent);

	mOutline=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outline"));
	mOutline->SetupAttachment(RootComponent);

	mScene=CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	mScene->SetupAttachment(RootComponent);

	mArrow=CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	mArrow->SetupAttachment(mScene);

	mHeartIcon=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeartIcon"));
	mHeartIcon->SetupAttachment(RootComponent);

	mHealthText=CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
	mHealthText->SetupAttachment(mHeartIcon);

	mAutoRotateToPlayer=CreateDefaultSubobject<UAutoRotateToPlayer>(TEXT("AutoRotateToPlayer"));

	mScaleHealthIcon=CreateDefaultSubobject<UTimelineComponent>(TEXT("ScaleHealthIcon"));
}

void ABoardPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerHealth);
	
	DOREPLIFETIME(ThisClass,mPlayerIndex);
	DOREPLIFETIME(ThisClass,mPlayerName);
}

void ABoardPlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	mPlayer->SetText(mPlayerName);

	if (GetOwner()
		&&GetOwner()->Implements<UControllerInterface>())
	{
		UMaterialInterface* mat=IControllerInterface::Execute_GetProfileImg(GetOwner());
		mPlayerTile->SetMaterial(0,mat);
	}
}

void ABoardPlayer::BeginPlay()
{
	Super::BeginPlay();
	FOnTimelineFloat scaleValue;
	scaleValue.BindDynamic(this,&ThisClass::TimelineProgress);
	FOnTimelineEvent finishedEvent;
	finishedEvent.BindDynamic(this,&ThisClass::TimelineFinished);
	if (mCurve)
	{
		mScaleHealthIcon->AddInterpFloat(mCurve,scaleValue);
	}
	mScaleHealthIcon->SetTimelineFinishedFunc(finishedEvent);
}

void ABoardPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void ABoardPlayer::OnRep_Health()
{
	if(!UCCGBFL::CanExecuteCosmeticEvents(this))
	{
		return;
	}
	mHealthText->SetText(FText::AsNumber(mPlayerHealth));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABoardPlayer::TimelineProgress(float Value)
{
	mHeartIcon->SetRelativeScale3D(mHeartIcon->GetRelativeScale3D()*Value);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABoardPlayer::TimelineFinished()
{
	mHealthText->SetTextRenderColor(FColor::Black);
}

void ABoardPlayer::Multicast_BoardPlayerEffect(EBoardPlayerEffects Effect)
{
	const int32 index=static_cast<int32>(Effect);
	UEffectBFL::SpawnParticleAndSoundEffect(this,mParticles[index],FVector::ZeroVector,this,FVector::OneVector,true,mSounds[index]);

	if (mScaleHealthIcon->IsPlaying()
		||mScaleHealthIcon->IsReversing())
	{
		return;
	}
	switch (Effect)
	{
	case EBoardPlayerEffects::DecreasedHealth:
		mHealthText->SetTextRenderColor(FColor::Yellow);
		break;
	case EBoardPlayerEffects::IncreasedHealth:
		mHealthText->SetTextRenderColor(FColor::White);
		break;
	case EBoardPlayerEffects::Death:
		break;
	default: ;
	}
	mScaleHealthIcon->PlayFromStart();
}

void ABoardPlayer::Server_UpdateHealth_Implementation(int32 Health)
{
	if (Health<=0)
	{
		Multicast_BoardPlayerEffect(EBoardPlayerEffects::Death);
	}
	else if (mPlayerHealth>Health)
	{
		Multicast_BoardPlayerEffect(EBoardPlayerEffects::DecreasedHealth);
	}
	else if (mPlayerHealth<Health)
	{
		Multicast_BoardPlayerEffect(EBoardPlayerEffects::IncreasedHealth);
	}
	mPlayerHealth=Health;
}
