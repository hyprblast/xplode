// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallSpawnerBase.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "xBallProjectileBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AxBallSpawnerBase::AxBallSpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
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
	LoadDynamicRefs();
	GameState = Cast<AxplodeGameStateBase>(GetWorld()->GetGameState());
}


void AxBallSpawnerBase::MulticastPlayBallSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayBallSoundFx, GetActorLocation());
}


void AxBallSpawnerBase::LoadDynamicRefs()
{
	PlayBallSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/_Main/SFX/PlayBall_Cue.PlayBall_Cue'")));
}

void AxBallSpawnerBase::SpawnBall()
{
	if (HasAuthority())
	{
		GameState->bShouldSpawnNewBall = false;
		GameState->bBallSpawned = true;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnWhere = GetActorTransform();//UKismetMathLibrary::MakeTransform(FVector(1534, 1467, 248), FRotator(0, 0, 0), FVector(1.0f, 1.0f, 1.0f));
	/*	SpawnWhere.SetScale3D(FVector(1.3f,1.3f,1.3f));*/

		AxBallBase* Ball = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), SpawnWhere, SpawnParams);
		MulticastPlayBallSound();
		Ball->Shoot(FVector(0, 0, 9000.f));
	}
	
}


void AxBallSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && GameState->bShouldSpawnNewBall)
	{
		SpawnBall();
	}

}

