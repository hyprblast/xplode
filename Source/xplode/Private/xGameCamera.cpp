// Fill out your copyright notice in the Description page of Project Settings.


#include "xGameCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "xBaseCharacterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "xBallProjectileBase.h"
#include "xBallBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetSystemLibrary.h"


AxGameCamera::AxGameCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	//SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	//SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//SphereComp->SetGenerateOverlapEvents(false);
	//SphereComp->CastShadow = false;
	//SphereComp->SetWorldLocation(FVector(0,0,80));
	///*SphereComp->SetNotifyRigidBodyCollision(true);*/

	//SphereComp->SetupAttachment(RootComponent);
	//
	//SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")));
	//
	//if (IsValid(SphereStaticMeshObject))
	//{
	//	SphereComp->SetStaticMesh(SphereStaticMeshObject);
	//}

	/*bReplicates = true;*/
}

void AxGameCamera::Tick(float DeltaTime)
{
	if (IsValid(FollowActor)) 
	{
		if (IsValid(FollowActor))
		{
			FVector CamLocation = GetActorLocation();
			FVector BallLocation = FollowActor->GetActorLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CamLocation, BallLocation);

			SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, 2.f));
			//SetActorLocation(FMath::Lerp(CamLocation, FVector(CamLocation.X, CamLocation.Y, CamLocation.Z), 0.01f));
		}
	}
}


