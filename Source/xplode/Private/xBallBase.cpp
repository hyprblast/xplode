// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallBase.h"
#include "xBaseCharacterInterface.h"
#include "../xplode.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Chaos/ChaosEngineInterface.h"
#include "xplodeGameStateBase.h"



// Sets default values
AxBallBase::AxBallBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
	SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
	SphereComp->SetGenerateOverlapEvents(false);
	SphereComp->CastShadow = false;
	/*SphereComp->SetLinearDamping(0);*/
	SphereComp->SetSimulatePhysics(false);
	SphereComp->SetNotifyRigidBodyCollision(true);


	/*SphereComp->SetNotifyRigidBodyCollision(true);*/


	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetIsReplicated(true);
	SphereComp->CanCharacterStepUp(false);
	/*SphereComp->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));*/


	SetRootComponent(SphereComp);

	bReplicates = true;
	SetReplicateMovement(true);

	Tags.Add("Ball");
}

void AxBallBase::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && IsValid(OtherActor))
	{
		if (OtherActor->ActorHasTag(FName("Player")) &&
			OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
			!IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
		{
			RemoveOverlapAndPhysics();
			IxBaseCharacterInterface::Execute_PickupBall(OtherActor, this);
			//Destroy();
		}
		else if (OtherActor->ActorHasTag("Goal") && !bScored)
		{
			bScored = true;
			MulticastPlayScoreSFX();

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &AxBallBase::DestroyAndSpawnNewBall, 3.f, false);

		}


	}
}


void AxBallBase::Shoot(FVector Force)
{
	if (!bIsExploding)
	{
		bIsExplodeMode = false;
		ExplodeLevel = 0;
		StopWarn();
		ClearTimer();
	}

	AddOverlapAndPhysics();
	SphereComp->AddImpulse(Force);
}

//void AxBallBase::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//	
//	// Attach ball on server version of the player so that it can be replicated
//	if (HasAuthority() &&
//		OtherActor->ActorHasTag(FName("Player")) &&
//		OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
//		!IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
//	{
//		//IxBaseCharacterInterface::Execute_PickupBall(OtherActor, this);
//		Destroy();
//		//IxBaseCharacterInterface::Execute_AttachBall(OtherActor);
//		//Destroy();
//
//	}
//
//}

// Called when the game starts or when spawned
void AxBallBase::BeginPlay()
{
	Super::BeginPlay();

	// VFX are not being replicated so load refs in client / server
	LoadDynamicRefs();

	GameState = Cast<AxplodeGameStateBase>(GetWorld()->GetGameState());

	// SphereComponent is replicated so just set the staticmesh on server
	if (HasAuthority())
	{
		SetStaticMesh();
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AxBallBase::CallOnOverlap);
		SphereComp->OnComponentHit.AddDynamic(this, &AxBallBase::OnCompHit);
		MulticastAddSelfAsCameraTarget();
	}
}

void AxBallBase::MulticastExplode_Implementation()
{

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSoundFx, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator(0, 0, 0));

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AxBallBase::SelfDestroy, 0.2f, false);

}


void AxBallBase::PlayWarn()
{
	OnWarning.Broadcast();
	/*AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WarningSoundFx, GetActorLocation());*/
}

void AxBallBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


	if (IsValid(OtherActor) && (OtherActor->ActorHasTag("Goal") || OtherActor->ActorHasTag("Wall")))
	{

		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			switch (SurfaceType)
			{
			case WALL:
				MulticastPlayBallHitWallFX();
					break;
			case POST:
				if (!bScored)
				{
					MulticastPlayBallHitPostSFX();

				}
				break;
			default:
				break;
			}

	}


}

void AxBallBase::MulticastAddSelfAsCameraTarget_Implementation()
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

void AxBallBase::MulticastPlayBallHitPostSFX_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitPostSoundFx, GetActorLocation());
}

void AxBallBase::MulticastPlayBallHitWallFX_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitWallSoundFx, GetActorLocation());
}

void AxBallBase::MulticastPlayScoreSFX_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ScoreSoundFx, GetActorLocation());
}

void AxBallBase::StopWarn()
{
	if (IsValid(AudioComponent))
	{
		AudioComponent->SetActive(false);
	}
}

void AxBallBase::MulticastSetOwnerNoSee_Implementation()
{
	SphereComp->SetOwnerNoSee(true);
}


void AxBallBase::OnTimerElapsed()
{
	ExplodeLevel += 1;

	if (ExplodeLevel == ExplodesAt - 2)
	{
		PlayWarn();
	}

	if (ExplodeLevel >= ExplodesAt && !bIsExploding)
	{
		bIsExploding = true;
		//StopWarn();
		// Clear timer
		ClearTimer();
		//// Explode
		MulticastExplode();
	}
}

void AxBallBase::LoadDynamicRefs()
{
	ExplosionParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Explosive/Explosion_GrenadeLauncher_1.Explosion_GrenadeLauncher_1'")));
	ExplosionSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/Battle_Royale_Game/Cues/Explosions/Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue.Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue'")));
	ScoreSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/_Main/SFX/Score_Cue.Score_Cue'")));
	HitPostSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/_Main/SFX/HitPost_Cue.HitPost_Cue'")));
	HitWallSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/_Main/SFX/HitWall_Cue.HitWall_Cue'")));
}

void AxBallBase::SetStaticMesh()
{
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/Orb/xOrb.xOrb'")));
	if (IsValid(SphereStaticMeshObject))
	{
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
	}
}


void AxBallBase::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplodeLevelIncrementTimerHandle);
}

void AxBallBase::StartTimer()
{
	bIsExplodeMode = true;
	GetWorldTimerManager().SetTimer(
		ExplodeLevelIncrementTimerHandle, this, &AxBallBase::OnTimerElapsed, 1.0f, true);
}


void AxBallBase::AddOverlapAndPhysics()
{
	SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
	SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetSimulatePhysics(true);
}

void AxBallBase::RemoveOverlapAndPhysics()
{
	SphereComp->SetSimulatePhysics(false);
	SphereComp->SetGenerateOverlapEvents(false);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AxBallBase::SelfDestroy()
{
	GetOwner()->Destroy();
	Destroy();

}

void AxBallBase::DestroyAndSpawnNewBall()
{
	GameState->bShouldSpawnNewBall = true;
	Destroy();
}

//Called every frame
//void AxBallBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AxBallBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxBallBase, bIsExplodeMode);
	DOREPLIFETIME(AxBallBase, ExplodeLevel);
	DOREPLIFETIME(AxBallBase, bIsExploding);
}

