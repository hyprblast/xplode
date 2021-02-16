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
#include "xplodeGameStateBase.h"

// Sets default values
AxBallProjectileBase::AxBallProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->SetIsReplicated(true);
	ProjectileMovementComp->bAutoActivate = false;


	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereComp->SetGenerateOverlapEvents(false);
	SphereComp->CastShadow = false;
	SphereComp->SetNotifyRigidBodyCollision(false);
	SphereComp->SetIsReplicated(true);
	
	/*UPhysicalMaterial* PM = Cast<UPhysicalMaterial>(StaticLoadObject(UPhysicalMaterial::StaticClass(), NULL, TEXT("PhysicalMaterial'/Game/_Main/GameStuff/NoFriction.NoFriction'")));
	SphereComp->SetPhysMaterialOverride(PM);*/

	SphereComp->SetupAttachment(RootComponent);

	//Cannot be stepped on
	SphereComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComp->CanCharacterStepUpOn = ECB_No;


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

void AxBallProjectileBase::MulticastPlayCoolDownSound_Implementation()
{
	AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), CoolDownSoundFx, GetActorLocation());
}

void AxBallProjectileBase::Shoot(FVector Velocity)
{
	bNeedsCoolDown = true;
	ProjectileMovementComp->Velocity = Velocity;
	/*ProjectileMovementComp->HomingAccelerationMagnitude = 50000;*/
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bRotationRemainsVertical = true;

	ProjectileMovementComp->bInterpMovement = true;
	ProjectileMovementComp->bInterpRotation = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.6f;
	ProjectileMovementComp->Activate();
	/*ProjectileMovementComp->MaxSpeed = 2000;
	ProjectileMovementComp->InitialSpeed = 2000;*/
	ProjectileMovementComp->ProjectileGravityScale = 0.7f;

	

}


void AxBallProjectileBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		ProjectileMovementComp->ProjectileGravityScale = 1;
		if (OtherActor->ActorHasTag(FName("Player")) &&
			OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
			!IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
		{
			
			if (ProjectileMovementComp->Velocity.IsNearlyZero() || IxBaseCharacterInterface::Execute_GetPlayerIsInCatchMode(OtherActor))
			{
				IxBaseCharacterInterface::Execute_PickupBall(OtherActor);
				Destroy();
			}
			
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

	// SphereComponent is replicated so just set the staticmesh on server
	if (HasAuthority())
	{
		SetStaticMesh();
		SphereComp->OnComponentHit.AddDynamic(this, &AxBallProjectileBase::OnCompHit);
	}
	else
	{
		AxplodeGameStateBase* GameState = Cast<AxplodeGameStateBase>(GetWorld()->GetGameState());

		if (IsValid(GameState) && IsValid(GameState->GameCamera))
		{
			GameState->GameCamera->FollowActor = this;
		}

	}

	LoadDynamicRefs();
}

void AxBallProjectileBase::LoadDynamicRefs()
{
	CoolDownSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Cooldown/SCIMisc_Cooldown_01_Cue.SCIMisc_Cooldown_01_Cue'")));
}


void AxBallProjectileBase::SetStaticMesh()
{
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/Orb/xOrb.xOrb'")));
	if (IsValid(SphereStaticMeshObject))
	{
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
	}
}


void AxBallProjectileBase::StopCoolDown()
{
	bNeedsCoolDown = false;
	
	if (IsValid(AudioComponent))
	{
		AudioComponent->SetActive(false);
	}
}

AxBallBase* AxBallProjectileBase::SpawnBall(bool bAddOverlap, AActor* OtherActor)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OtherActor;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	AxBallBase* Ball = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), GetTransform(), SpawnParams);
	if (bAddOverlap)
	{
		Ball->AddOverlap();
	}
	
	GetWorld()->GetTimerManager().ClearTimer(CoolDownTimerHandle);
	return Ball; 
}



