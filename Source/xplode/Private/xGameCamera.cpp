// Fill out your copyright notice in the Description page of Project Settings.


#include "xGameCamera.h"
#include "Kismet/KismetMathLibrary.h"


AxGameCamera::AxGameCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AxGameCamera::Tick(float DeltaTime)
{
	if (IsValid(FollowActor))
	{
		FVector CamLocation = GetActorLocation();
		FVector ActorLocation = FollowActor->GetActorLocation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CamLocation, ActorLocation);

		SetActorRotation(LookAtRotation);
	}
}
