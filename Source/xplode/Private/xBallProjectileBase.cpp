// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallProjectileBase.h"
#include "../xplode.h"
#include "Chaos/ChaosEngineInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"
#include "Components/PrimitiveComponent.h"
#include "xBallBase.h"
#include "UObject/UObjectGlobals.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "xBaseCharacterInterface.h"
#include <UObject/ConstructorHelpers.h>
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "xplodeGameStateBase.h"

// Sets default values
AxBallProjectileBase::AxBallProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollisionComp->SetGenerateOverlapEvents(false);
	SphereCollisionComp->CastShadow = false;
	SphereCollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereCollisionComp->CanCharacterStepUpOn = ECB_No;
	SphereCollisionComp->SetNotifyRigidBodyCollision(true);
	SphereCollisionComp->SetCollisionObjectType(XBALLPROJECTILEOBJECT_CHANNEL);
	SphereCollisionComp->SetCollisionProfileName(TEXT("xBallProjectileCollision"));

	SphereCollisionComp->SetIsReplicated(true);
	SphereCollisionComp->SetSphereRadius(20.f);



	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->SetIsReplicated(true);
	ProjectileMovementComp->bAutoActivate = false;
	


	SphereStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereStaticMeshComp->SetGenerateOverlapEvents(false);
	SphereStaticMeshComp->CastShadow = false;
	SphereStaticMeshComp->SetNotifyRigidBodyCollision(false);
	SphereStaticMeshComp->SetIsReplicated(true);
	SphereStaticMeshComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereStaticMeshComp->CanCharacterStepUpOn = ECB_No;
	SphereStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);



	// center mesh on sphere collision
	SphereStaticMeshComp->SetWorldLocation(FVector(0, 10.5f, 0));

	/*UPhysicalMaterial* PM = Cast<UPhysicalMaterial>(StaticLoadObject(UPhysicalMaterial::StaticClass(), NULL, TEXT("PhysicalMaterial'/Game/_Main/GameStuff/NoFriction.NoFriction'")));
	SphereStaticMeshComp->SetPhysMaterialOverride(PM);*/

	SphereStaticMeshComp->SetupAttachment(SphereCollisionComp);

	//Cannot be stepped on
	SphereStaticMeshComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereStaticMeshComp->CanCharacterStepUpOn = ECB_No;


	SetRootComponent(SphereCollisionComp);

	bReplicates = true;
	SetReplicateMovement(true);


	ProjectileMovementComp->UpdatedComponent = SphereCollisionComp;
	ProjectileMovementComp->SetInterpolatedComponent(SphereStaticMeshComp);
	ProjectileMovementComp->bInterpMovement = true;
	ProjectileMovementComp->bInterpRotation = true;
	/*ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bRotationRemainsVertical = false;*/
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.6f;
	


	/*NetUpdateFrequency = 2000.f;*/
	//MinNetUpdateFrequency = 20.f;

}

void AxBallProjectileBase::RemoveCollision()
{
	SphereCollisionComp->SetNotifyRigidBodyCollision(false);
	SphereCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


//void AxBallProjectileBase::MulticastPlayCoolDownSound_Implementation()
//{
//	AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), CoolDownSoundFx, GetActorLocation());
//}

void AxBallProjectileBase::Shoot(FVector Velocity)
{
	ProjectileVelocity = Velocity;
	ProjectileMovementComp->Velocity = ProjectileVelocity;
	/*ProjectileMovementComp->HomingAccelerationMagnitude = 50000;*/



	ProjectileMovementComp->Activate();
	/*ProjectileMovementComp->MaxSpeed = 2000;
	ProjectileMovementComp->InitialSpeed = 2000;*/
	ProjectileMovementComp->ProjectileGravityScale = 0.8f;
	

}


void AxBallProjectileBase::MulticastAddSelfAsCameraTarget_Implementation()
{
	TArray<AActor*> FoundCameras;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AxGameCamera::StaticClass(), FoundCameras);

	if (FoundCameras.Num() == 2)
	{
		AxGameCamera* Cam1 = Cast<AxGameCamera>(FoundCameras[0]);
		AxGameCamera* Cam2 = Cast<AxGameCamera>(FoundCameras[1]);

		Cam1->FollowActor = this;
		Cam2->FollowActor = this;
	}
}

void AxBallProjectileBase::AutoDestroyAfterSecs(float Seconds)
{
	GetWorldTimerManager().SetTimer(
		AutoDestroyTimerHandle, this, &AxBallProjectileBase::AutoDestroy, Seconds, false);
}

//void AxBallProjectileBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	if (HasAuthority())
//	{
//		MulticastServerTransform(GetActorTransform(), ProjectileMovementComp->Velocity);
//	}
//}

void AxBallProjectileBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority() && IsValid(OtherActor))
	{
		
		if (OtherActor->ActorHasTag(FName("Player")) &&
			OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
			!IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
		{
			ProjectileMovementComp->ProjectileGravityScale = 1;
			IxBaseCharacterInterface::Execute_PickupBall(OtherActor);
			Destroy();
			/*if (ProjectileMovementComp->Velocity.IsNearlyZero() || IxBaseCharacterInterface::Execute_GetPlayerIsInCatchMode(OtherActor))
			{
				
			}*/

			//IxBaseCharacterInterface::Execute_AttachBall(OtherActor);
			//Destroy();

		}


		//if (OtherActor->ActorHasTag(FName("Player")))
		//{
		//	// Catch ball directly if facing ball
		//	IxBaseCharacterInterface::Execute_PickupBall(OtherActor);
		//	Destroy();
		//	//EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		//}
			//else 
			//{
			//	if (!bNeedsCoolDown)
			//	{
			//		AxBallProjectileBase::SpawnBall(true, OtherActor);
			//		Destroy();
			//	}
			//	else
			//	{
			//		if (!bCoolDownStarted)
			//		{
			//			bCoolDownStarted = true;
			//			// Cool down time is fixed for now but might be a good idea to make it dependent on velocity
			//			GetWorldTimerManager().SetTimer(
			//				CoolDownTimerHandle, this, &AxBallProjectileBase::StopCoolDown, 2.f, false);

			//			MulticastPlayCoolDownSound();
			//		}

			//	}
			//}





	}

	//if (HasAuthority() && OtherActor->ActorHasTag(FName("Player")) && !bMarkedForDestroy)
	//	{
	//		

	//		// Destroy and spawn ball
	//	/*GetWorldTimerManager().SetTimer(
	//		SpawnBallTimerHandle, this, &AxBallProjectileBase::DestroyAndSpawnBall, 1.0f, true);*/
	//	}
}

// Called when the game starts or when spawned
void AxBallProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// SphereStaticMeshComponent is replicated so just set the staticmesh on server
	if (HasAuthority())
	{
		SetStaticMesh();
		SphereCollisionComp->OnComponentHit.AddDynamic(this, &AxBallProjectileBase::OnCompHit);
		MulticastAddSelfAsCameraTarget();
	}

	/*LoadDynamicRefs();*/
}

void AxBallProjectileBase::PostNetReceiveLocationAndRotation()
{
	/*if ((GetReplicatedMovement().Location == GetActorLocation() && GetReplicatedMovement().Rotation == GetActorRotation()) && (CreationTime != GetWorld()->TimeSeconds))
	{
		return;
	}*/

	ProjectileMovementComp->MoveInterpolationTarget(GetReplicatedMovement().Location, GetReplicatedMovement().Rotation);
}

void AxBallProjectileBase::MulticastServerTransform_Implementation(FTransform ServerTransform, FVector ServerVelocity)
{
	SetActorTransform(ServerTransform);
	ProjectileMovementComp->Velocity = ServerVelocity;
}

//void AxBallProjectileBase::LoadDynamicRefs()
//{
//	CoolDownSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Cooldown/SCIMisc_Cooldown_01_Cue.SCIMisc_Cooldown_01_Cue'")));
//}


void AxBallProjectileBase::SetStaticMesh()
{
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/Orb/xOrb.xOrb'")));
	if (IsValid(SphereStaticMeshObject))
	{
		SphereStaticMeshComp->SetStaticMesh(SphereStaticMeshObject);
	}
}


void AxBallProjectileBase::AutoDestroy()
{
	GetWorld()->GetTimerManager().ClearTimer(AutoDestroyTimerHandle);
	Destroy();
}


void AxBallProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxBallProjectileBase, ProjectileVelocity);
}


