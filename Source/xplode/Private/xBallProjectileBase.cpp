// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallProjectileBase.h"
#include "../xplode.h"

// Sets default values
AxBallProjectileBase::AxBallProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/xprotoball.xprotoball'")));

	if (IsValid(SphereStaticMeshObject))
	{
		ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
		ProjectileMovementComp->Velocity = FVector(2000, 0, 0);
		ProjectileMovementComp->HomingAccelerationMagnitude = 2000;
		ProjectileMovementComp->bRotationFollowsVelocity = true;
		/*ProjectileMovementComp->ProjectileGravityScale = 0;*/
		ProjectileMovementComp->SetIsReplicated(true);

		SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
		SphereComp->SetCollisionObjectType(XBALLPROJECTILEOBJECT_CHANNEL);
		SphereComp->SetCollisionProfileName(TEXT("xBallProjectileCollision"));
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
		SphereComp->SetupAttachment(RootComponent);
		SphereComp->SetIsReplicated(true);
		SphereComp->CanCharacterStepUp(false);
		SetRootComponent(SphereComp);
		
		bReplicates = true;
		SetReplicateMovement(true);
	}


}

// Called when the game starts or when spawned
void AxBallProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AxBallProjectileBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

