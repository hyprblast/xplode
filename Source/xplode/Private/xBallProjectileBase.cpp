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

// Sets default values
AxBallProjectileBase::AxBallProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	/*SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/xprotoball.xprotoball'")));*/
	/*ImpactParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Explosive/Explosion_GrenadeLauncher_1.Explosion_GrenadeLauncher_1'")));
	ImpactSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/Battle_Royale_Game/Cues/Explosions/Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue.Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue'")));*/


	//if (IsValid(SphereStaticMeshObject))
	//{
	//	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//	ProjectileMovementComp->Velocity = FVector(2000, 0, 0);
	//	ProjectileMovementComp->HomingAccelerationMagnitude = 2000;
	//	ProjectileMovementComp->bRotationFollowsVelocity = true;
	//	/*ProjectileMovementComp->ProjectileGravityScale = 0;*/
	//	ProjectileMovementComp->SetIsReplicated(true);


	//	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	//	SphereComp->SetSimulatePhysics(true);
	//	SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
	//	SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
	//	/*SphereComp->SetNotifyRigidBodyCollision(true);*/
	//	SphereComp->SetGenerateOverlapEvents(false);
	//	SphereComp->SetStaticMesh(SphereStaticMeshObject);
	//	SphereComp->SetupAttachment(RootComponent);
	//	SphereComp->SetIsReplicated(true);
	//	SphereComp->CanCharacterStepUp(false);
	//	SetRootComponent(SphereComp);

	//	bReplicates = true;
	//	SetReplicateMovement(true);
	//}


}

//void AxBallProjectileBase::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	
//	/*if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("AxBallProjectileBase Overlap"));
//	}*/
//
//	/*UE_LOG(LogTemp, Log, TEXT("Other Actor is: %s"), *OtherComp->GetFullName());*/
//}

//void AxBallProjectileBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Projectile HIt"));
//	}
//	
//	// Hit something
//	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
//
//	switch (SurfaceType)
//	{
//	case SurfaceType_Default:
//		/*if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Default"));
//		}*/
//		break;
//	case SurfaceType1:
//		break;
//	case SurfaceType2:
//		break;
//	case SurfaceType3:
//		break;
//	case SurfaceType4:
//		break;
//	case SurfaceType5:
//		break;
//	case SurfaceType6:
//		break;
//	case SurfaceType7:
//		break;
//	case SurfaceType8:
//		break;
//	case SurfaceType9:
//		break;
//	case SurfaceType10:
//		break;
//	case SurfaceType11:
//		break;
//	case SurfaceType12:
//		break;
//	case SurfaceType13:
//		break;
//	case SurfaceType14:
//		break;
//	case SurfaceType15:
//		break;
//	case SurfaceType16:
//		break;
//	case SurfaceType17:
//		break;
//	case SurfaceType18:
//		break;
//	case SurfaceType19:
//		break;
//	case SurfaceType20:
//		break;
//	case SurfaceType21:
//		break;
//	case SurfaceType22:
//		break;
//	case SurfaceType23:
//		break;
//	case SurfaceType24:
//		break;
//	case SurfaceType25:
//		break;
//	case SurfaceType26:
//		break;
//	case SurfaceType27:
//		break;
//	case SurfaceType28:
//		break;
//	case SurfaceType29:
//		break;
//	case SurfaceType30:
//		break;
//	case SurfaceType31:
//		break;
//	case SurfaceType32:
//		break;
//	case SurfaceType33:
//		break;
//	case SurfaceType34:
//		break;
//	case SurfaceType35:
//		break;
//	case SurfaceType36:
//		break;
//	case SurfaceType37:
//		break;
//	case SurfaceType38:
//		break;
//	case SurfaceType39:
//		break;
//	case SurfaceType40:
//		break;
//	case SurfaceType41:
//		break;
//	case SurfaceType42:
//		break;
//	case SurfaceType43:
//		break;
//	case SurfaceType44:
//		break;
//	case SurfaceType45:
//		break;
//	case SurfaceType46:
//		break;
//	case SurfaceType47:
//		break;
//	case SurfaceType48:
//		break;
//	case SurfaceType49:
//		break;
//	case SurfaceType50:
//		break;
//	case SurfaceType51:
//		break;
//	case SurfaceType52:
//		break;
//	case SurfaceType53:
//		break;
//	case SurfaceType54:
//		break;
//	case SurfaceType55:
//		break;
//	case SurfaceType56:
//		break;
//	case SurfaceType57:
//		break;
//	case SurfaceType58:
//		break;
//	case SurfaceType59:
//		break;
//	case SurfaceType60:
//		break;
//	case SurfaceType61:
//		break;
//	case SurfaceType62:
//		break;
//	case SurfaceType_Max:
//		break;
//	default:
//		break;
//	}
//
//	if (HasAuthority())
//	{
//		/*if (IsValid(ImpactSoundFx) && IsValid(ImpactParticle))
//		{
//			MulticastSelfDestruct();
//		}*/
//
//		/*SphereComp->SetSimulatePhysics(true);*/
//		/*SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
//		SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
//		SphereComp->SetGenerateOverlapEvents(true);*/
//
//		SphereComp->OnComponentHit.RemoveDynamic(this, &AxBallProjectileBase::OnCompHit);
//	
//	/*	RemoveDynamic(SphereComp, &AxBallProjectileBase::OnCompHit);*/
//		//Destroy();
//
//		//TODO: Spanw new xBall
//	}
//}

// Called when the game starts or when spawned
void AxBallProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	/*if (HasAuthority())
	{
		SphereComp->OnComponentHit.AddDynamic(this, &AxBallProjectileBase::OnCompHit);
	}*/

	/*SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AxBallProjectileBase::CallOnOverlap);*/
}


//void AxBallProjectileBase::BlastAndDestroyAfter(FVector EmiterSpawnLocation, FRotator EmiterSpawnRotation, EPhysicalSurface SurfaceType, float time)
//{
//	
//}

// When called from server will be executed locally and on all instances of this actor
//void AxBallProjectileBase::MulticastSelfDestruct_Implementation()
//{
//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundFx, GetActorLocation());
//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation(), FRotator(0, 0, 0));
//
//	FTimerHandle UnusedHandle;
//	GetWorldTimerManager().SetTimer(
//		UnusedHandle, this, &AxBallProjectileBase::SelfDestroy, 0.2, false);
//}

//void AxBallProjectileBase::SelfDestroy()
//{
//	Destroy();
//}

// Called every frame
//void AxBallProjectileBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

