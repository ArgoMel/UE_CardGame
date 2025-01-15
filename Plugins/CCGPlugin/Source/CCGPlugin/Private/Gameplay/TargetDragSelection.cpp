// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TargetDragSelection.h"

#include "BFL/MiscBFL.h"
#include "Kismet/KismetMathLibrary.h"

ATargetDragSelection::ATargetDragSelection()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	mToLocation=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToLocation"));
	mToLocation->SetupAttachment(RootComponent);

	mTarget=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	mTarget->SetupAttachment(mToLocation);

	mNarrowCapsule=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NarrowCapsule"));
	mNarrowCapsule->SetupAttachment(RootComponent);
}

void ATargetDragSelection::BeginPlay()
{
	Super::BeginPlay();
}

void ATargetDragSelection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATargetDragSelection::SetSelectionProperties(FVector Location, bool ValidTarget) const
{
	mTarget->SetVisibility(ValidTarget);

	const FVector actorLoc=GetActorLocation();
	const float length=(Location-actorLoc).Length();
	APlayerController* playerController=Cast<APlayerController>(GetOwner());
	FHitResult hitResult;
	UMiscBFL::MouseToWorldLocation(playerController,hitResult);
	const FRotator lookAtRot=UKismetMathLibrary::FindLookAtRotation(actorLoc,hitResult.Location);
	const FRotator newRot=FRotator(0.f,lookAtRot.Yaw+90.f,lookAtRot.Pitch);

	FTransform newTransform;
	newTransform.SetLocation(actorLoc);
	newTransform.SetRotation(newRot.Quaternion());
	newTransform.SetScale3D(FVector(1.5f,length/195.f,length/500.f));
	
	mNarrowCapsule->SetWorldTransform(newTransform);
	
	newTransform.SetLocation(Location);
    newTransform.SetScale3D(FVector(2.f));
	mToLocation->SetWorldTransform(newTransform);
}

