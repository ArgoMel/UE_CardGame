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
	//�ڽ��� �� ������Ʈ�� ���� ���͸� �����ؼ� �����Ϳ��� ����ְ�
	//���⼭ �ش� ������ �ڽ��� �� �ȿ��� ������ x,y��ġ�� �޾ƿ��� �����°� ���Ѵ�
	SetActorLocation(offset);
	mDiceMesh->AddImpulse(FVector(FMath::FRandRange(1000.f, 10000.f), 
		FMath::FRandRange(1000.f, 10000.f), FMath::FRandRange(1000.f, 10000.f)));
}

