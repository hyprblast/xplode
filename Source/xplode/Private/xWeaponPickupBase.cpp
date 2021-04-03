// Fill out your copyright notice in the Description page of Project Settings.


#include "xWeaponPickupBase.h"
#include "../xplode.h"
#include "Net/UnrealNetwork.h"
#include "xBaseCharacterInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AxWeaponPickupBase::AxWeaponPickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SceneComp->SetupAttachment(RootComponent);

	LightComp = CreateDefaultSubobject<URectLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(SceneComp);
	LightComp->CastShadows = false;
	LightComp->SetHiddenInGame(true);

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxCollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollisionComp->SetGenerateOverlapEvents(true);
	BoxCollisionComp->SetNotifyRigidBodyCollision(false);
	BoxCollisionComp->SetupAttachment(SceneComp);
	
	CrateComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CrateComp")); 
	CrateComp->SetCollisionProfileName(TEXT("BlockAll"));
	CrateComp->SetGenerateOverlapEvents(false);
	CrateComp->SetNotifyRigidBodyCollision(false);
	CrateComp->CastShadow = false;
	CrateComp->SetIsReplicated(true);
	CrateComp->CanCharacterStepUpOn = ECB_No;
	CrateComp->SetupAttachment(SceneComp);

	WeaponPickupComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponPickupComp"));
	WeaponPickupComp->SetCollisionObjectType(XWEAPONPICKUPOBJECT_CHANNEL);
	WeaponPickupComp->SetCollisionProfileName(TEXT("xWeaponPickupCollision"));
	WeaponPickupComp->CastShadow = false;
	WeaponPickupComp->SetIsReplicated(true);
	WeaponPickupComp->CanCharacterStepUpOn = ECB_No;
	WeaponPickupComp->SetupAttachment(SceneComp);
	WeaponPickupComp->SetHiddenInGame(true);


	SetRootComponent(SceneComp);

	bReplicates = true;
	SetReplicateMovement(false);

	Tags.Add("Crate");


}

// Called when the game starts or when spawned
void AxWeaponPickupBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxWeaponPickupBase::CallOnOverlap);
		BoxCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AxWeaponPickupBase::OnOverlapEnd);
	}
}


void AxWeaponPickupBase::Open()
{
	bIsOpen = true;
	CrateComp->GetAnimInstance()->Montage_Play(OpenCrateMontage);
	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AxWeaponPickupBase::OpenMontageOnAnimationEnd);
	CrateComp->GetAnimInstance()->Montage_SetBlendingOutDelegate(CompleteDelegate, OpenCrateMontage);
}

void AxWeaponPickupBase::OpenMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("End")); 
	WeaponPickupComp->SetHiddenInGame(false);
	LightComp->SetHiddenInGame(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LootAuraParticle, GetActorLocation(), FRotator(0, 0, 0));
}

void AxWeaponPickupBase::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")) &&
		OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass())
		&& !IxBaseCharacterInterface::Execute_GetPlayerIsDead(OtherActor)
		&& !IxBaseCharacterInterface::Execute_GetPlayerIsKO(OtherActor)
		&& !bTriggerOpen)
	{
		bTriggerOpen = true;

		IxBaseCharacterInterface::Execute_SetWeaponToSpawn(OtherActor, this, WeaponBase);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Press right thumnbstick to pickup weapon"));
	}
}

void AxWeaponPickupBase::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Player")) &&
		OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass())
		&& !IxBaseCharacterInterface::Execute_GetPlayerIsDead(OtherActor)
		&& !IxBaseCharacterInterface::Execute_GetPlayerIsKO(OtherActor)
		&& bTriggerOpen)
	{
		IxBaseCharacterInterface::Execute_SetWeaponToSpawn(OtherActor, this, NULL);
		bTriggerOpen = false;
	}
	
}

// Called every frame
void AxWeaponPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTriggerOpen && !bIsOpen)
	{
		Open();
	}

	if (bIsOpen)
	{
		FRotator CurrentRotation = WeaponPickupComp->GetRelativeRotation();
		FRotator TargetRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + 45.f, CurrentRotation.Roll), DeltaTime, 2.f);

		WeaponPickupComp->SetRelativeRotation(TargetRotation);
	}
}

void AxWeaponPickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AxWeaponPickupBase, bIsPickedUp);
	DOREPLIFETIME(AxWeaponPickupBase, bTriggerOpen);
	
}

