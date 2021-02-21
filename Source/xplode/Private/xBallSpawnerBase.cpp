// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallSpawnerBase.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "xBallProjectileBase.h"

// Sets default values
AxBallSpawnerBase::AxBallSpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//SpawnerStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/SuperGrid/StarterPack/Source/Meshes/SuperGrid_RingFull.SuperGrid_RingFull'")));

	//if (IsValid(SpawnerStaticMeshObject))
	//{
	//	SpawnerComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawner"));
	//	SpawnerComp->CanCharacterStepUp(false);
	//	SpawnerComp->SetStaticMesh(SpawnerStaticMeshObject);
	//	SpawnerComp->SetupAttachment(RootComponent);
	//	SetRootComponent(SpawnerComp);
	//}

}

// Called when the game starts or when spawned
void AxBallSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnBall();
}

void AxBallSpawnerBase::SpawnBall()
{
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnWhere = GetActorTransform();//UKismetMathLibrary::MakeTransform(FVector(1534, 1467, 248), FRotator(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));

		AxBallProjectileBase* BallProjectile = GetWorld()->SpawnActor<AxBallProjectileBase>(AxBallProjectileBase::StaticClass(), SpawnWhere, SpawnParams);
		BallProjectile->AddSelfAsCameraTarget();
		BallProjectile->AddCollision();
		BallProjectile->Shoot(FVector(0, -100, 800));
	}
	
}



// Called every frame
//void AxBallSpawnerBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

