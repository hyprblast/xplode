// Fill out your copyright notice in the Description page of Project Settings.


#include "xBallBase.h"
#include "xBaseCharacterInterface.h"
#include "../xplode.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AxBallBase::AxBallBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereStaticMeshObject = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/_Main/StaticMeshes/xprotoball.xprotoball'")));
	
	if (IsValid(SphereStaticMeshObject))
	{
		SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
		SphereComp->SetCollisionObjectType(XBALLOBJECT_CHANNEL);
		SphereComp->SetCollisionProfileName(TEXT("xBallCollision"));
		SphereComp->SetGenerateOverlapEvents(true);
		SphereComp->SetStaticMesh(SphereStaticMeshObject);
		SphereComp->SetupAttachment(RootComponent);
		/*SphereComp->SetSimulatePhysics(true);*/
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

void AxBallBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// Attach ball on server version of the player so that it can be replicated
	if (HasAuthority() && 
		OtherActor->ActorHasTag(FName("Player")) && 
		OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) && 
		!IxBaseCharacterInterface::Execute_GetPlayerHasBall(OtherActor))
	{
		SetOwner(OtherActor);
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

// Called when the game starts or when spawned
void AxBallBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
//void AxBallBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

//void AxBallBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AxBallBase, LastPlayerOwner);
//}

