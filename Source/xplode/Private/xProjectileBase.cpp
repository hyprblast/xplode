// Fill out your copyright notice in the Description page of Project Settings.

#include "xProjectileBase.h"
#include "xBaseCharacterInterface.h"
#include "Delegates/Delegate.h"
#include "Kismet/GameplayStatics.h"
#include "PistolShootDamageType.h"
#include "../xplode.h"

// Sets default values
AxProjectileBase::AxProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollisionComp->SetGenerateOverlapEvents(true);
	SphereCollisionComp->CastShadow = false;
	SphereCollisionComp->SetNotifyRigidBodyCollision(false);
	SphereCollisionComp->SetCollisionObjectType(XPROJECTILEOBJECT_CHANNEL);
	SphereCollisionComp->SetCollisionProfileName(TEXT("xOverlapSkeletalMeshCollision"));
	SphereCollisionComp->SetIsReplicated(false);
	SphereCollisionComp->SetSphereRadius(20.f);
	//SphereCollisionComp->SetUseCCD(true);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->SetIsReplicated(false);
	ProjectileMovementComp->bAutoActivate = false;
	ProjectileMovementComp->InitialSpeed = 5000.f;
	ProjectileMovementComp->MaxSpeed = 0;
	ProjectileMovementComp->ProjectileGravityScale = 0;
	
	/*ProjectileMovementComp->UpdatedComponent = SphereCollisionComp;*/
	//ProjectileMovementComp->SetInterpolatedComponent(ProjectileParticleComp);
	//ProjectileMovementComp->bInterpMovement = true;
	//ProjectileMovementComp->bInterpRotation = true;
	/*ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bRotationRemainsVertical = false;*/
	ProjectileMovementComp->bShouldBounce = false;
	/*ProjectileMovementComp->Bounciness = 0.6f;*/


	ProjectileParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticleComp"));
	ProjectileParticleComp->CastShadow = false;
	ProjectileParticleComp->SetIsReplicated(false);
	ProjectileParticleComp->SetupAttachment(SphereCollisionComp);

	SetRootComponent(SphereCollisionComp);


	bReplicates = false;
	SetReplicateMovement(false);

}

void AxProjectileBase::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && !bHasHit && GetWorld()->IsServer()) // server spawn projectiles
	{
		bHasHit = true;
		
		if (OtherActor->ActorHasTag(FName("Player")) &&
			OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass())
			&& !IxBaseCharacterInterface::Execute_GetPlayerIsDead(OtherActor)
			&& !IxBaseCharacterInterface::Execute_GetPlayerIsKO(OtherActor)
			&& OtherActor != GetOwner())
		{
			/*UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), 10, 12, FLinearColor::Red, 5.f, 3.f);*/
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, SweepResult.BoneName.ToString());

			IxBaseCharacterInterface::Execute_SetShotBoneName(OtherActor, SweepResult.BoneName);
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

			UPistolShootDamageType* PistolShotDamageType = NewObject<UPistolShootDamageType>();
			UGameplayStatics::ApplyPointDamage(OtherActor, PistolShotDamageType->Damage, SweepResult.ImpactPoint, SweepResult, GetOwner()->GetInstigatorController(), GetOwner(), PistolShotDamageType->StaticClass());

		}
	}
	else
	{
		// client projectiles will just spawn emmiter and be discarded
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
		Destroy();
	}
}


void AxProjectileBase::Shoot(FVector Velocity)
{
	/*ProjectileVelocity = Velocity;*/
	ProjectileMovementComp->Velocity = Velocity;
	ProjectileMovementComp->Activate();
	
}

// Called when the game starts or when spawned
void AxProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) 
	{
		SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxProjectileBase::CallOnOverlap);
	}

}

// Called every frame
//void AxProjectileBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

//void AxProjectileBase::PostNetReceiveLocationAndRotation()
//{
//	ProjectileMovementComp->MoveInterpolationTarget(GetReplicatedMovement().Location, GetReplicatedMovement().Rotation);
//}
//
//void AxProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AxProjectileBase, ProjectileVelocity);
//}

