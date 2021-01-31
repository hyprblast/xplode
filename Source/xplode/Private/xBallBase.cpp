// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallBase.h"
#include "xBaseCharacterInterface.h"
#include "../xplode.h"
#include <Components/PrimitiveComponent.h>

// Sets default values
AxBallBase::AxBallBase()
{
	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/xprotoball.xprotoball'")));
	
	if (SphereStaticMeshObject && SphereStaticMeshObject != nullptr)
	{
		SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
		SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
		SphereComp->SetupAttachment(RootComponent);
	/*	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AxBallBase::CallOnOverlap);*/
		SetRootComponent(SphereComp);

	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
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
	/*UE_LOG(LogTemp, Log, TEXT("Other Actor is: %s"), *OtherActor->GetFullName());*/
	// Attach client version of ball to player. Bc it replicates all other player will see the ball
	if (!HasAuthority() && 
		OtherActor->ActorHasTag(FName("Player")) && 
		OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
	    !IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
	{
			IxBaseCharacterInterface::Execute_SetPlayerHasBall(OtherActor, true);
			IxBaseCharacterInterface::Execute_AttachBall(OtherActor, this);
	}
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

