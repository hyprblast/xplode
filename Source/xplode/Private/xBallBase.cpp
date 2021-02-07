// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallBase.h"
#include "xBaseCharacterInterface.h"
#include "../xplode.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/ProjectileMovementComponent.h>


// Sets default values
AxBallBase::AxBallBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/xprotoball.xprotoball'")));
	ExplosionParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Explosive/Explosion_GrenadeLauncher_1.Explosion_GrenadeLauncher_1'")));
	ExplosionSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/Battle_Royale_Game/Cues/Explosions/Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue.Explosion_Grenade_Close_2_Bomb_Explode_Fiery_Loud_Cue'")));


	if (IsValid(SphereStaticMeshObject))
	{
		ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
		/*ProjectileMovementComp->Velocity = FVector(2000, 0, 0);
		ProjectileMovementComp->HomingAccelerationMagnitude = 2000;
		ProjectileMovementComp->bRotationFollowsVelocity = true;*/
		/*ProjectileMovementComp->ProjectileGravityScale = 0;*/
		ProjectileMovementComp->SetIsReplicated(true);
		
		
		SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
		SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
		SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
		SphereComp->SetGenerateOverlapEvents(true);
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
		SphereComp->SetupAttachment(RootComponent);
		SphereComp->SetSimulatePhysics(false);
		SphereComp->SetIsReplicated(true);
		SphereComp->CanCharacterStepUp(false);
		
		/*SphereComp->SetUseCCD(true);*/
	/*	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AxBallBase::CallOnOverlap);*/
		SetRootComponent(SphereComp);

		bReplicates = true;
		SetReplicateMovement(true);
	}

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

//void AxBallProjectileBase::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//
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
		
		bIsExplodeMode = true;
		StartTimer();
		
		/*SphereComp->WakeAllRigidBodies();
		SphereComp->SetOwnerNoSee(true);*/
		/*SetOwner(OtherActor);*/

		SetOwner(OtherActor);
		MulticastSetOwnerNoSee();


		IxBaseCharacterInterface::Execute_AttachBall(OtherActor, this);

		if (LastPlayerOwner && LastPlayerOwner != nullptr)
		{
			IxBaseCharacterInterface::Execute_DetachBall(LastPlayerOwner, this);
		}
		/*else
		{
			UE_LOG(LogTemp, Log, TEXT("Last Player Owner is null"));
		}*/

		LastPlayerOwner = OtherActor;

	}
}

void AxBallBase::Shoot(FVector Velocity, float AccelerationMag)
{
	ProjectileMovementComp->Velocity = Velocity;
	/*ProjectileMovementComp->HomingAccelerationMagnitude = 50000;*/
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;
	/*ProjectileMovementComp->MaxSpeed = 2000;*/
	/*ProjectileMovementComp->ProjectileGravityScale = 0;*/
}

// Called when the game starts or when spawned
void AxBallBase::BeginPlay()
{
	Super::BeginPlay();
}

void AxBallBase::MulticastExplode_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSoundFx, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), FRotator(0, 0, 0));

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AxBallBase::SelfDestroy, 0.2f, false);

}


void AxBallBase::MulticastSetOwnerNoSee_Implementation()
{
	SphereComp->SetOwnerNoSee(true);
}

void AxBallBase::OnTimerElapsed()
{
	ExplodeLevel += 1;

	if (ExplodeLevel >= ExplodesAt && !bIsExploding)
	{
		bIsExploding = true;

		// Clear timer
		ClearTimer();
		// Explode
		MulticastExplode();
	}
}

void AxBallBase::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ExplodeLevelIncrementTimerHandle);
}

void AxBallBase::StartTimer()
{
	GetWorldTimerManager().SetTimer(
		ExplodeLevelIncrementTimerHandle, this, &AxBallBase::OnTimerElapsed, 1.0f, true);
}

void AxBallBase::SelfDestroy()
{
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
}

