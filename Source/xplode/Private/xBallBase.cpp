// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallBase.h"

// Sets default values
AxBallBase::AxBallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void AxBallBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Make sure server is handling collision and not the client
	if (HasAuthority())
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{

		}
	}

	UE_LOG(LogTemp, Log, TEXT("Overlap"));
}

// Called when the game starts or when spawned
void AxBallBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AxBallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

