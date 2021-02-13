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


// Sets default values
AxBallBase::AxBallBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
	SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
	SphereComp->SetGenerateOverlapEvents(false);
	SphereComp->CastShadow = false;
	/*SphereComp->SetNotifyRigidBodyCollision(true);*/


	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetIsReplicated(true);
	SphereComp->CanCharacterStepUp(false);

	SetRootComponent(SphereComp);

	bReplicates = true;
	SetReplicateMovement(true);
	

}

//void AxBallBase::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	/*if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("NotHello %s"), *OtherComp->GetFullName());
//	}*/
//
//	UE_LOG(LogTemp, Log, TEXT("Other Actor is: %s"), *OtherComp->GetFullName());
//}


void AxBallBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// Attach ball on server version of the player so that it can be replicated
	if (HasAuthority() &&
		OtherActor->ActorHasTag(FName("Player")) &&
		OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
		!IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
	{
		IxBaseCharacterInterface::Execute_PickupBall(OtherActor);
		Destroy();
		//IxBaseCharacterInterface::Execute_AttachBall(OtherActor);
		//Destroy();

	}

}

// Called when the game starts or when spawned
void AxBallBase::BeginPlay()
{
	Super::BeginPlay();

	// VFX are not being replicated so load refs in client / server
	LoadVFXDynamicRefs();

	// SphereComponent is replicated so just set the staticmesh on server
	if (HasAuthority())
	{
		SetStaticMesh();
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


void AxBallBase::ClientWarn_Implementation()
{
	AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WarningSoundFx, GetActorLocation());
}

void AxBallBase::ClientStopWarn_Implementation()
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

	if (ExplodeLevel == ExplodesAt -2)
	{
		ClientWarn();
	}

	if (ExplodeLevel >= ExplodesAt && !bIsExploding)
	{
		bIsExploding = true;
		ClientStopWarn();
		// Clear timer
		ClearTimer();
		//// Explode
		MulticastExplode();
	}
}

void AxBallBase::LoadVFXDynamicRefs()
{
	ExplosionParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Explosive/Explosion_GrenadeLauncher_1.Explosion_GrenadeLauncher_1'")));
	ExplosionSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/Battle_Royale_Game/Cues/Explosions/Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue.Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue'")));
	WarningSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Targeting/BEEP_Targeting_Loop_04_Cue.BEEP_Targeting_Loop_04_Cue'")));
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

void AxBallBase::AddOverlap()
{
	SphereComp->SetGenerateOverlapEvents(true);
}

void AxBallBase::SelfDestroy()
{
	GetOwner()->Destroy();
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

