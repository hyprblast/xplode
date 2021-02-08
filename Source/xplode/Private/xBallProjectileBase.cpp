// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallProjectileBase.h"
#include "../xplode.h"
#include "Chaos/ChaosEngineInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include <Delegates/Delegate.h>
#include <Components/PrimitiveComponent.h>
#include "xBallBase.h"
#include "UObject/UObjectGlobals.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AxBallProjectileBase::AxBallProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->SetIsReplicated(true);

	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereComp->SetGenerateOverlapEvents(false);
	SphereComp->CastShadow = false;
	SphereComp->SetNotifyRigidBodyCollision(false);
	SphereComp->SetIsReplicated(true);

	SphereComp->SetupAttachment(RootComponent);
	SphereComp->CanCharacterStepUp(false);

	SetRootComponent(SphereComp);

	bReplicates = true;
	SetReplicateMovement(true);

	ProjectileMovementComp->UpdatedComponent = SphereComp;

}

void AxBallProjectileBase::AddCollision()
{
	SphereComp->SetCollisionObjectType(XBALLPROJECTILEOBJECT_CHANNEL);
	SphereComp->SetCollisionProfileName(TEXT("xBallProjectileCollision"));
	SphereComp->SetNotifyRigidBodyCollision(true);
	
}

void AxBallProjectileBase::Shoot(FVector Velocity, float AccelerationMag)
{
	ProjectileMovementComp->Velocity = Velocity;
	/*ProjectileMovementComp->HomingAccelerationMagnitude = 50000;*/
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;
	/*ProjectileMovementComp->MaxSpeed = 2000;
	ProjectileMovementComp->InitialSpeed = 2000;*/
	/*ProjectileMovementComp->ProjectileGravityScale = 0;*/
}


void AxBallProjectileBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (HasAuthority() && OtherActor->ActorHasTag(FName("Floor")) && !bMarkedForDestroy)
		{
			bMarkedForDestroy = true;

			// Destroy and spawn ball
		GetWorldTimerManager().SetTimer(
			SpawnBallTimerHandle, this, &AxBallProjectileBase::DestroyAndSpawnBall, 1.0f, true);
		}
}

// Called when the game starts or when spawned
void AxBallProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// SphereComponent is replicated so just set the staticmesh on server
	if (HasAuthority())
	{
		SetStaticMesh();
		SphereComp->OnComponentHit.AddDynamic(this, &AxBallProjectileBase::OnCompHit);
	}
}


void AxBallProjectileBase::SetStaticMesh()
{
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/AdvancedWeaponPack/Weapons/Meshes/FragGrenade/StaticMeshes/SM_FragGrenade_Grenade.SM_FragGrenade_Grenade'")));
	if (IsValid(SphereStaticMeshObject))
	{
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
	}
}


void AxBallProjectileBase::DestroyAndSpawnBall()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	AxBallBase* Ball = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), GetTransform(), SpawnParams);
	Ball->AddOverlap();
	GetWorld()->GetTimerManager().ClearTimer(SpawnBallTimerHandle);
	Destroy();
}


