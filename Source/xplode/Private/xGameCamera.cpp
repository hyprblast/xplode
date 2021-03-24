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
#include "Math/UnrealMathUtility.h"


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
			//FRotator CamRotation = GetActorRotation();

			FVector BallLocation = FollowActor->GetActorLocation();
			//FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CamLocation, BallLocation);

			/*float TargetYaw = CamRotation.Yaw + 45.f * TurnYaw;
			TargetYaw = FMath::Clamp(TargetYaw, 45.f, 135.f);*/

			/*if (TurnYaw == 0)
			{
				TargetYaw = 90.f;
			}*/

			/*GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Yellow, FString::SanitizeFloat(TargetYaw));*/

			
			//SetActorRotation(UKismetMathLibrary::RInterpTo(CamRotation, FRotator(CamRotation.Pitch, TargetYaw, CamInitialRotation.Roll), DeltaTime, 2.f));




			/*if (bShouldTurn && TurnYaw != CamRotation.Yaw)
			{
				
			}*/
			/*else
			{
				if (!bShouldTurn)
				{
					FRotator CurrentCamRotation = GetActorRotation();
					SetActorRotation(UKismetMathLibrary::RInterpTo(CurrentCamRotation, CamInitialRotation, DeltaTime, 2.f));

					TurnYaw = CamInitialRotation.Yaw;
				}
			}*/

			/*SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), FRotator(CamRotation.Pitch, LookAtRotation.Yaw, CamRotation.Roll), DeltaTime, 2.f));*/
			SetActorLocation(FMath::Lerp(CamLocation, FVector(BallLocation.X, CamLocation.Y, CamLocation.Z), 0.02f));
		}
	}
}



//void AxGameCamera::SetTurnYaw(float TurnYawVal)
//{
//	TurnYaw = TurnYawVal;
//	bShouldTurn = true;
//}

//void AxGameCamera::BeginPlay()
//{
//	Super::BeginPlay();
//	//CamInitialRotation = GetActorRotation();
//}

