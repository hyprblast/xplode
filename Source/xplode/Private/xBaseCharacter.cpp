// Fill out your copyright notice in the Description page of Project Settings.


#include "xBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "xBallBase.h"
#include "../xplodeGameModeBase.h"
#include "GeneratedCodeHelpers.h"
#include "CollisionQueryParams.h"
#include "../xplode.h"
#include "Math/Color.h"
#include "DrawDebugHelpers.h"
#include "Chaos/ChaosEngineInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include "xBallProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AxBaseCharacter::AxBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();

	/*CapsuleComp->SetGenerateOverlapEvents(false);*/
	/*GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetWorldLocation(FVector(0, 0, 70.0f));
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	SkeletalMeshComp->SetupAttachment(CameraComp);

	Tags.Add(FName("Player"));

	// Load References
	ThrowBallMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/ThrowBallAnimMontage.ThrowBallAnimMontage'")));
	ThrowBallMontageTPV = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/ThrowBallAnimMontageTPV.ThrowBallAnimMontageTPV'")));

}

bool AxBaseCharacter::GetPlayerIsThrowing_Implementation()
{
	return bIsThrowing;
}

bool AxBaseCharacter::GetPlayerHasBall_Implementation()
{
	return bHasBall;
}


int32 AxBaseCharacter::SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing)
{
	ServerSetPLayerIsThrowing(bPlayerIsThrowing);
	return 1;
}

int32 AxBaseCharacter::ThrowBall_Implementation()
{
	ServerThrowBall(CameraComp->GetComponentLocation(), CameraComp->GetForwardVector());
	return 1;
}

float AxBaseCharacter::GetInputAxisYawValue_Implementation()
{
	return InputAxisYawValue;
}

int32 AxBaseCharacter::DetachBall_Implementation(AxBallBase* Ball)
{
	bHasBall = false;
	DestroyBalls();
	return 1;
}

int32 AxBaseCharacter::AttachBall_Implementation(AxBallBase* Ball)
{

	AttachBallToTPVMesh(Ball);
	SpawnNewBallOnFPVMesh();

	return 1;
}

// Called when the game starts or when spawned
void AxBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AxBaseCharacter::HandleTakeAnyDamage);
}

void AxBaseCharacter::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

}

// Called every frame
//void AxBaseCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AxBaseCharacter::MoveFoward(float Value)
{
	FVector FowardVector = GetActorForwardVector();
	AddMovementInput(FowardVector, Value);
}

void AxBaseCharacter::MoveRight(float Value)
{
	FVector RightVector = GetActorRightVector();
	AddMovementInput(RightVector, Value);
}

void AxBaseCharacter::AttachBallToTPVMesh(AxBallBase* Ball)
{
	bHasBall = true;

	FName SocketName = TEXT("hand_socket");
	USkeletalMeshComponent* TPVSkeletalMesh = GetMesh();

	/*Ball->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	/*Ball->SphereComp->SetGenerateOverlapEvents(false);*/
	/*Ball->SetOwner(this);*/
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform TPVSocketTransform = TPVSkeletalMesh->GetSocketTransform(SocketName, RTS_World);

	/*Ball->SphereComp->SetOwnerNoSee(true);*/
	Ball->AttachToComponent(TPVSkeletalMesh, TransformRules, SocketName);

	TPVBall = Ball;

	/*Ball->SetActorTransform(TPVSocketTransform);*/
}


void AxBaseCharacter::Turn(float Value)
{
	InputAxisYawValue = Value;
	AxBaseCharacter::AddControllerYawInput(Value);
}

void AxBaseCharacter::PlayThrowBallAnim()
{
	if (bHasBall)
	{
		bIsThrowing = true;
		SkeletalMeshComp->GetAnimInstance()->Montage_Play(ThrowBallMontage);
		ServerPlayTPVThrowBallAnim();
	}
}

void AxBaseCharacter::ServerPlayTPVThrowBallAnim_Implementation()
{
	bIsThrowing = true;
	MulticastPlayTPVThrowAnimation();
}

bool AxBaseCharacter::ServerPlayTPVThrowBallAnim_Validate()
{
	return IsValid(TPVBall);
}

void AxBaseCharacter::ServerThrowBall_Implementation(FVector CameraLocation, FVector CameraFowardVector)
{
	
	FHitResult Hit;
	/*FName SocketName = TEXT("hand_socket");*/
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraFowardVector * 10000);
	/*FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName, RTS_World);*/
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TraceEnd);
	FTransform ThrowTo = UKismetMathLibrary::MakeTransform(CameraLocation, LookAtRotation, FVector(1.0f, 1.0f, 1.0f));
	

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	/*DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);*/

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, XBALTRACE_CHANNEL, QueryParams))
	{
		// Hit something
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		float DamageToApply = 0;

		AActor* HitActor = Hit.GetActor();

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, TEXT("HEAD"));
		}*/

		switch (SurfaceType)
		{
		case SurfaceType_Default:
			break;
		case HEAD:
			DamageToApply = TPVBall->Damage * 4.0f;
			break;
		case TORSO:
			DamageToApply = TPVBall->Damage * 2.0f;
			break;
		case ARMS:
			DamageToApply = TPVBall->Damage * 0.6f;
			break;
		case LEGS:
			DamageToApply = TPVBall->Damage * 0.7f;
			break;
		case SurfaceType5:
			break;
		case SurfaceType6:
			break;
		case SurfaceType7:
			break;
		case SurfaceType8:
			break;
		case SurfaceType9:
			break;
		case SurfaceType10:
			break;
		case SurfaceType11:
			break;
		case SurfaceType12:
			break;
		case SurfaceType13:
			break;
		case SurfaceType14:
			break;
		case SurfaceType15:
			break;
		case SurfaceType16:
			break;
		case SurfaceType17:
			break;
		case SurfaceType18:
			break;
		case SurfaceType19:
			break;
		case SurfaceType20:
			break;
		case SurfaceType21:
			break;
		case SurfaceType22:
			break;
		case SurfaceType23:
			break;
		case SurfaceType24:
			break;
		case SurfaceType25:
			break;
		case SurfaceType26:
			break;
		case SurfaceType27:
			break;
		case SurfaceType28:
			break;
		case SurfaceType29:
			break;
		case SurfaceType30:
			break;
		case SurfaceType31:
			break;
		case SurfaceType32:
			break;
		case SurfaceType33:
			break;
		case SurfaceType34:
			break;
		case SurfaceType35:
			break;
		case SurfaceType36:
			break;
		case SurfaceType37:
			break;
		case SurfaceType38:
			break;
		case SurfaceType39:
			break;
		case SurfaceType40:
			break;
		case SurfaceType41:
			break;
		case SurfaceType42:
			break;
		case SurfaceType43:
			break;
		case SurfaceType44:
			break;
		case SurfaceType45:
			break;
		case SurfaceType46:
			break;
		case SurfaceType47:
			break;
		case SurfaceType48:
			break;
		case SurfaceType49:
			break;
		case SurfaceType50:
			break;
		case SurfaceType51:
			break;
		case SurfaceType52:
			break;
		case SurfaceType53:
			break;
		case SurfaceType54:
			break;
		case SurfaceType55:
			break;
		case SurfaceType56:
			break;
		case SurfaceType57:
			break;
		case SurfaceType58:
			break;
		case SurfaceType59:
			break;
		case SurfaceType60:
			break;
		case SurfaceType61:
			break;
		case SurfaceType62:
			break;
		case SurfaceType_Max:
			break;
		default:
			break;
		}
		
		// Spawn Projectile Ball
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AxBallProjectileBase* BallProjectile = GetWorld()->SpawnActor<AxBallProjectileBase>(AxBallProjectileBase::StaticClass(), ThrowTo, SpawnParams);

		/*TPVBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);*/
	/*	TPVBall->SphereComp->SetSimulatePhysics(true);
		TPVBall->SphereComp->AddImpulse(TraceEnd);*/

		if (DamageToApply > 0)
		{
			UGameplayStatics::ApplyPointDamage(HitActor, DamageToApply, CameraFowardVector, Hit, GetOwner()->GetInstigatorController(), TPVBall, TPVBall->DamageType);
		}

		DestroyBalls();
	}
}

bool AxBaseCharacter::ServerThrowBall_Validate(FVector CameraLocation, FVector CameraFowardVector)
{
	return IsValid(TPVBall);
}

void AxBaseCharacter::ServerSetPLayerIsThrowing_Implementation(bool bPlayerIsThrowing)
{
	bIsThrowing = bPlayerIsThrowing;
	bHasBall = bIsThrowing;
}

bool AxBaseCharacter::ServerSetPLayerIsThrowing_Validate(bool bPlayerIsThrowing)
{
	return true;
}

void AxBaseCharacter::MulticastPlayTPVThrowAnimation_Implementation()
{
	// Play on all instances of this actor
	if (IsValid(ThrowBallMontageTPV))
	{
		bIsThrowing = true;
		GetMesh()->GetAnimInstance()->Montage_Play(ThrowBallMontageTPV);
	}
}

// Called to bind functionality to input
void AxBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AxBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AxBaseCharacter::Turn);

	PlayerInputComponent->BindAxis("MoveFoward", this, &AxBaseCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AxBaseCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AxBaseCharacter::Jump);
	PlayerInputComponent->BindAction("ThrowBall", IE_Pressed, this, &AxBaseCharacter::PlayThrowBallAnim);
}


void AxBaseCharacter::SpawnNewBallOnFPVMesh()
{
	FName SocketName = TEXT("hand_socket");

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform FPVSocketTransform = SkeletalMeshComp->GetSocketTransform(SocketName, RTS_World);

	FPVBall = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), FPVSocketTransform, spawnParams);
	FPVBall->SphereComp->SetGenerateOverlapEvents(false);
	FPVBall->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPVBall->SphereComp->SetOnlyOwnerSee(true);
	FPVBall->SetReplicates(false);
	FPVBall->SetReplicateMovement(false);
	FPVBall->AttachToComponent(SkeletalMeshComp, TransformRules, SocketName);
	FPVBall->SetOwner(this);
}

void AxBaseCharacter::DestroyBalls()
{
	if (IsValid(FPVBall) && IsValid(TPVBall))
	{
		FPVBall->Destroy();
		TPVBall->Destroy();
	}
}

void AxBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxBaseCharacter, bHasBall);
	DOREPLIFETIME(AxBaseCharacter, FPVBall);
	DOREPLIFETIME(AxBaseCharacter, TPVBall);
	DOREPLIFETIME(AxBaseCharacter, bIsThrowing);
}

