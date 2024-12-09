#include "Dice/Dice.h"

ADice::ADice()
{
	PrimaryActorTick.bCanEverTick = false;

	mDiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiceMesh"));
	SetRootComponent(mDiceMesh);
	mDiceMesh->SetRelativeScale3D(FVector(0.1));
	mDiceMesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Dice(TEXT(
		"/Script/Engine.StaticMesh'/Game/Dice/Mesh/SM_Dice.SM_Dice'"));
	if (SM_Dice.Succeeded())
	{
		mDiceMesh->SetStaticMesh(SM_Dice.Object);
	}
}

void ADice::BeginPlay()
{
	Super::BeginPlay();
}

void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADice::GetRandomLocation()
{
	return FVector();
}

void ADice::CalculateDiceResult()
{
	float dir = GetActorForwardVector().Z;
	if (dir == 1.f)
	{
		mRollingResult = 2;
	}
	else if (dir == -1.f)
	{
		mRollingResult = 5;
	}
	dir = GetActorUpVector().Z;
	if (dir == 1.f)
	{
		mRollingResult = 4;
	}
	else if (dir == -1.f)
	{
		mRollingResult = 3;
	}
	dir = GetActorRightVector().Z;
	if (dir == 1.f)
	{
		mRollingResult = 1;
	}
	else if (dir == -1.f)
	{
		mRollingResult = 6;
	}
}

void ADice::SendRollingResult(int32 result)
{
	if(result!=0)
	{
		mRollingResult = result;
	}
}

void ADice::CheckStillRolling_Implementation()
{
	if (GetActorLocation() != mDiceLoc)
	{
		mDiceLoc = GetActorLocation();
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(mDiceTimer);
	CalculateDiceResult();
	UE_LOG(LogTemp, Warning, TEXT("Dice is Stop : %i"), mRollingResult);
}

void ADice::Rolling_Implementation()
{
	float randRange[3];
	for (int32 i = 0; i < 3;++i)
	{
		randRange[i] = FMath::FRandRange(-180.f, 180.f);
	}
	SetActorRotation(FRotator(randRange[0], randRange[1], randRange[2]));
	const FVector offset(0.,0.,700.);
	SetActorLocation(offset);
	mDiceMesh->AddImpulse(FVector(randRange[0], randRange[1], randRange[2])*10.);
	//mDiceMesh->AddImpulse(FVector(FMath::FRandRange(1000.f, 10000.f), 
	//	FMath::FRandRange(1000.f, 10000.f), FMath::FRandRange(1000.f, 10000.f)));
	mDiceLoc = GetActorLocation();
	GetWorld()->GetTimerManager().SetTimer(mDiceTimer, this, &ADice::CheckStillRolling, 0.5f, true);
}