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

void ADice::Rolling_Implementation()
{
	SetActorRotation(FRotator(
		FMath::FRandRange(-180.f, 180.f), FMath::FRandRange(-180.f, 180.f), FMath::FRandRange(-180.f, 180.f)));
	FVector offset(0.,0.,700.);
	//박스나 원 컴포넌트를 갖는 액터를 생성해서 에디터에서 집어넣고
	//여기서 해당 액터의 박스나 원 안에서 램덤한 x,y위치를 받아오고 오프셋과 더한다
	SetActorLocation(offset);
	mDiceMesh->AddImpulse(FVector(FMath::FRandRange(1000.f, 10000.f), 
		FMath::FRandRange(1000.f, 10000.f), FMath::FRandRange(1000.f, 10000.f)));
}

