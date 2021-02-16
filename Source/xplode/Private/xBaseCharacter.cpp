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
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "xBallProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Engine/Blueprint.h>
#include "xplodeGameStateBase.h"


// Sets default values
AxBaseCharacter::AxBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->CanCharacterStepUp(false);

	//CapsuleComp->SetWalkableSlopeOverride()
	/*CapsuleComp->SetCollisionProfileName(TEXT("Spectator"));*/
	/*CapsuleComp->SetGenerateOverlapEvents(false);*/

	USkeletalMeshComponent* TPVMesh = GetMesh();

	TPVMesh->SetCollisionObjectType(XBALLNOCOLLISIONOBJECT_CHANNEL);
	TPVMesh->SetCollisionProfileName(TEXT("xBalllMeshNoCollision"));
	TPVMesh->CanCharacterStepUp(false);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetWorldLocation(FVector(0, 0, 70.0f));
	/*CameraComp->bUsePawnControlRotation = false;*/
	CameraComp->SetupAttachment(CapsuleComp);

	


	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	SkeletalMeshComp->CanCharacterStepUp(false);
	SkeletalMeshComp->SetupAttachment(CameraComp);


	SkeletalMeshComp->SetCollisionObjectType(XBALLNOCOLLISIONOBJECT_CHANNEL);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("xBalllMeshNoCollision"));
	SkeletalMeshComp->CanCharacterStepUp(false);


	/*GetCharacterMovement()->PushForceFactor =0;
	GetCharacterMovement()->InitialPushForceFactor = 0;
	GetCharacterMovement()->MaxTouchForce = 0;
	GetCharacterMovement()->RepulsionForce = 0;
	GetCapsuleComponent()->SetSimulatePhysics(false);*/

	Tags.Add(FName("Player"));

	
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> GameCameraBP(TEXT("Blueprint'/Game/_Main/Actors/Blueprints/BP_GameCamera.BP_GameCamera'"));
	GameCamera = Cast<AxGameCamera>(GameCameraBP.Object);*/

}

bool AxBaseCharacter::GetPlayerIsThrowing_Implementation()
{
	return bIsThrowing;
}

bool AxBaseCharacter::GetPlayerHasBall_Implementation()
{
	return bHasBall;
}


bool AxBaseCharacter::GetPlayerIsInCatchMode_Implementation()
{
	return bIsCatchMode;
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

int32 AxBaseCharacter::PickupBall_Implementation()
{
	MulticastPlayTPVPickupAnimation();
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

int32 AxBaseCharacter::AttachBall_Implementation()
{
	if (HasAuthority())
	{
		AttachBallToTPVMesh();
	}
	else if (IsLocallyControlled())
	{
		SpawnNewBallOnFPVMesh();
	}

	return 1;
}

// Called when the game starts or when spawned
void AxBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadDynamicRefs();

	OnTakeAnyDamage.AddDynamic(this, &AxBaseCharacter::HandleTakeAnyDamage);

	if (IsLocallyControlled())
	{
		CameraVieww = UxCameraView::TopDown;

		FTimerHandle UUnusedHandle;
		GetWorldTimerManager().SetTimer(
			UUnusedHandle, this, &AxBaseCharacter::SetTopDownCamera, 0.01f, false);
	}
}

//void AxBaseCharacter::Restart()
//{
//	// Restart on server happens after possess.
//	if (HasAuthority())
//	{
//		ClientSetTopDownCamera();
//	}
//}

void AxBaseCharacter::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

}

// Called every frame
void AxBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bIsAddingThrowPower && ThrowPower < MaxThrowPower)
		{
			ThrowPower += 25;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(ThrowPower));
			}
		}
	}
	
}

void AxBaseCharacter::MoveFoward(float Value)
{
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction * Value);
}

void AxBaseCharacter::MoveRight(float Value)
{
	/*FVector RightVector = GetActorRightVector();
	AddMovementInput(RightVector, Value);*/

	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement in that direction
	AddMovementInput(Direction, Value);
}

void AxBaseCharacter::AttachBallToTPVMesh()
{
	bHasBall = true;
	/*bIsCatchMode = false;
	bIsAddingThrowPower = false;*/
	/*bIsThrowing = false;*/

	FName SocketName = TEXT("hand_socket");
	USkeletalMeshComponent* TPVSkeletalMesh = GetMesh();

	/*Ball->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	/*Ball->SphereComp->SetGenerateOverlapEvents(false);*/
	/*Ball->SetOwner(this);*/
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform TPVSocketTransform = TPVSkeletalMesh->GetSocketTransform(SocketName, RTS_World);

	TPVBall = SpawnBall(TPVSocketTransform);
	TPVBall->MulticastSetOwnerNoSee();

	TPVBall->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TPVBall->AttachToComponent(TPVSkeletalMesh, TransformRules, SocketName);
	/*TPVBall->StartTimer();*/

	/*Ball->SetActorTransform(TPVSocketTransform);*/
}

void AxBaseCharacter::SetFPVCamera()
{

	//Find the actor that handles control for the local player.
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	//Smoothly transition to our actor on begin play.
	OurPlayerController->SetViewTargetWithBlend(this, .5f);

	CameraComp->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	
}


void AxBaseCharacter::SetTopDownCamera()
{
	AxplodeGameStateBase* GameState = Cast<AxplodeGameStateBase>(GetWorld()->GetGameState());

	if (IsValid(GameState) && IsValid(GameState->GameCamera))
	{
		/*Find the actor that handles control for the local player.*/
		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

		//Smoothly transition to top down camera.
		OurPlayerController->SetViewTargetWithBlend(GameState->GameCamera, .5f);

		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;

		//UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
		//if (MovementComponent)
		//{
		//	MovementComponent->bOrientRotationToMovement = true;
		//	/*MovementComponent->bUseControllerDesiredRotation = false;*/
		///*	MovementComponent->RotationRate = FRotator(0.0f,540.0f, 0.0f);*/
		//	/*MovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = true;*/

		//}

		/*GetCharacterMovement()->bOrientRotationToMovement = true;

		bUseControllerRotationYaw = false;*/

		/*GetCharacterMovement()->bUseControllerDesiredRotation = true;

		GetMesh()->SetRelativeRotation(CombatModeCharacterRotation);*/
	}
	

	/*if (!IsValid(GameCamera))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("NO CAMERA"));
	}*/


	SkeletalMeshComp->SetHiddenInGame(true);
	GetMesh()->bOwnerNoSee = false;
}

void AxBaseCharacter::TempChangeCollision(AxBallProjectileBase* BallProjectile)
{
	BallProjectile->AddCollision();
}

AxBallBase* AxBaseCharacter::SpawnBall(FTransform SpawnLocation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AxBallBase* Ball = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), SpawnLocation, SpawnParams);

	return Ball;
}

void AxBaseCharacter::LoadDynamicRefs()
{
	// Load References
	ThrowBallMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/ThrowBallAnimMontage.ThrowBallAnimMontage'")));
	ThrowBallMontageTPV = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/ThrowBallAnimMontageTPV.ThrowBallAnimMontageTPV'")));
	PickupBallMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PickUpAnimMontage.PickUpAnimMontage'")));
	ThrowPowerIncreaseSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Charge/SCIMisc_Charge_Weapon_15_Cue.SCIMisc_Charge_Weapon_15_Cue'")));
	//GameCamera = Cast<AxGameCamera>(StaticLoadObject(UBlueprint::StaticClass(), NULL, TEXT("Blueprint'/Game/_Main/Actors/Blueprints/BP_GameCamera.BP_GameCamera'")));
}

void AxBaseCharacter::Turn(float Value)
{
	if (CameraVieww == UxCameraView::FirstPerson)
	{
		InputAxisYawValue = Value;
		AxBaseCharacter::AddControllerYawInput(Value);
	}
	
}

void AxBaseCharacter::PlayThrowBallAnim()
{

	if (!bHasBall)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("NO BALL"));
	}

	if (bIsThrowing)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("THROWING"));
	}

	if (ThrowPower <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("THROW POWER <= 0"));
	}

	if (bHasBall && !bIsThrowing && ThrowPower > 0)
	{
		bIsThrowing = true;
		SkeletalMeshComp->GetAnimInstance()->Montage_Play(ThrowBallMontage);
		ServerPlayTPVThrowBallAnim();
	}
}

void AxBaseCharacter::UnSetCatchMode()
{
	bIsCatchMode = false;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Catch Mode OFF"));

	GetWorld()->GetTimerManager().ClearTimer(CatchModeTimerHandle);
}


void AxBaseCharacter::CamToggle()
{
	switch (CameraVieww)
	{
	case UxCameraView::FirstPerson:
		CameraVieww = UxCameraView::TopDown;
		SetTopDownCamera();
		break;
	case UxCameraView::TopDown:
		CameraVieww = UxCameraView::FirstPerson;
		SetFPVCamera();
		break;
	default:
		break;
	}
}

void AxBaseCharacter::ServerSetCatchMode_Implementation()
{

	if (!bIsCatchMode)
	{
		bIsCatchMode = true;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Catch Mode oN"));
		}

		GetWorldTimerManager().SetTimer(
			CatchModeTimerHandle, this, &AxBaseCharacter::UnSetCatchMode, 2.f, false);
	}
}

bool AxBaseCharacter::ServerSetCatchMode_Validate()
{
	return true;
}

//void AxBaseCharacter::ClientSetTopDownCamera_Implementation()
//{
//	SetTopDownCamera();
//}

void AxBaseCharacter::IncreaseThrowPower()
{
	if (bHasBall && !bIsThrowing)
	{
		if (!bIsAddingThrowPower)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ThrowPowerIncreaseSoundFx, GetActorLocation());
		}

		ServerIncreaseThrowPower();
	}
}

void AxBaseCharacter::ServerIncreaseThrowPower_Implementation()
{

	if (bHasBall)
	{
		bIsAddingThrowPower = true;
	}

}

bool AxBaseCharacter::ServerIncreaseThrowPower_Validate()
{
	return true;
}

void AxBaseCharacter::ServerPlayTPVThrowBallAnim_Implementation()
{
	bIsAddingThrowPower = false;
	bIsThrowing = true;
	MulticastPlayTPVThrowAnimation();
}

bool AxBaseCharacter::ServerPlayTPVThrowBallAnim_Validate()
{
	return IsValid(TPVBall) && !TPVBall->bIsExploding;
}

void AxBaseCharacter::MulticastPlayTPVPickupAnimation_Implementation()
{
	if (IsValid(PickupBallMontage))
	{
		if (IsLocallyControlled())
		{
			SkeletalMeshComp->GetAnimInstance()->Montage_Play(PickupBallMontage);
		}
		else
		{
			GetMesh()->GetAnimInstance()->Montage_Play(PickupBallMontage);
		}
	}
}


void AxBaseCharacter::ServerThrowBall_Implementation(FVector CameraLocation, FVector CameraFowardVector)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("ServerThrowBall_Implementation"));
	}


	FHitResult Hit;
	/*FName SocketName = TEXT("hand_socket");*/
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraFowardVector * 10000);
	/*FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName, RTS_World);*/
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TraceEnd);
	FTransform ThrowTo = UKismetMathLibrary::MakeTransform(CameraLocation, LookAtRotation, FVector(1.0f, 1.0f, 1.0f));


	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	AxBallProjectileBase* ProjectileBall = GetWorld()->SpawnActor<AxBallProjectileBase>(AxBallProjectileBase::StaticClass(), ThrowTo, SpawnParams);
	/*ProjectileBall->AddCollision();*/
	ProjectileBall->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Ignore the spawner of this projectile
	ProjectileBall->SphereComp->MoveIgnoreActors.Add(this);
	ProjectileBall->AddCollision();
	ProjectileBall->Shoot(CameraFowardVector * (ThrowPower > MaxThrowPower ? MaxThrowPower : ThrowPower));
	DestroyBalls();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::SanitizeFloat(ThrowPower));
	}


	/*TimerDel.BindUFunction(this, FName("TempChangeCollision"), ProjectileBall);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.04f, false);*/


	/*FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AxBaseCharacter::DestroyBalls, 0.2f, false);*/

		/*DestroyBalls();*/

		/*FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;*/

		/*DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);*/

		//if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, XBALTRACE_CHANNEL, QueryParams))
		//{
		//	// Hit something
		//	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		//	float DamageToApply = 0;

		//	AActor* HitActor = Hit.GetActor();

		//	/*if (GEngine)
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, TEXT("HEAD"));
		//	}*/

		//	switch (SurfaceType)
		//	{
		//	case SurfaceType_Default:
		//		break;
		//	case HEAD:
		//		DamageToApply = TPVBall->Damage * 4.0f;
		//		break;
		//	case TORSO:
		//		DamageToApply = TPVBall->Damage * 2.0f;
		//		break;
		//	case ARMS:
		//		DamageToApply = TPVBall->Damage * 0.6f;
		//		break;
		//	case LEGS:
		//		DamageToApply = TPVBall->Damage * 0.7f;
		//		break;
		//	case SurfaceType5:
		//		break;
		//	case SurfaceType6:
		//		break;
		//	case SurfaceType7:
		//		break;
		//	case SurfaceType8:
		//		break;
		//	case SurfaceType9:
		//		break;
		//	case SurfaceType10:
		//		break;
		//	case SurfaceType11:
		//		break;
		//	case SurfaceType12:
		//		break;
		//	case SurfaceType13:
		//		break;
		//	case SurfaceType14:
		//		break;
		//	case SurfaceType15:
		//		break;
		//	case SurfaceType16:
		//		break;
		//	case SurfaceType17:
		//		break;
		//	case SurfaceType18:
		//		break;
		//	case SurfaceType19:
		//		break;
		//	case SurfaceType20:
		//		break;
		//	case SurfaceType21:
		//		break;
		//	case SurfaceType22:
		//		break;
		//	case SurfaceType23:
		//		break;
		//	case SurfaceType24:
		//		break;
		//	case SurfaceType25:
		//		break;
		//	case SurfaceType26:
		//		break;
		//	case SurfaceType27:
		//		break;
		//	case SurfaceType28:
		//		break;
		//	case SurfaceType29:
		//		break;
		//	case SurfaceType30:
		//		break;
		//	case SurfaceType31:
		//		break;
		//	case SurfaceType32:
		//		break;
		//	case SurfaceType33:
		//		break;
		//	case SurfaceType34:
		//		break;
		//	case SurfaceType35:
		//		break;
		//	case SurfaceType36:
		//		break;
		//	case SurfaceType37:
		//		break;
		//	case SurfaceType38:
		//		break;
		//	case SurfaceType39:
		//		break;
		//	case SurfaceType40:
		//		break;
		//	case SurfaceType41:
		//		break;
		//	case SurfaceType42:
		//		break;
		//	case SurfaceType43:
		//		break;
		//	case SurfaceType44:
		//		break;
		//	case SurfaceType45:
		//		break;
		//	case SurfaceType46:
		//		break;
		//	case SurfaceType47:
		//		break;
		//	case SurfaceType48:
		//		break;
		//	case SurfaceType49:
		//		break;
		//	case SurfaceType50:
		//		break;
		//	case SurfaceType51:
		//		break;
		//	case SurfaceType52:
		//		break;
		//	case SurfaceType53:
		//		break;
		//	case SurfaceType54:
		//		break;
		//	case SurfaceType55:
		//		break;
		//	case SurfaceType56:
		//		break;
		//	case SurfaceType57:
		//		break;
		//	case SurfaceType58:
		//		break;
		//	case SurfaceType59:
		//		break;
		//	case SurfaceType60:
		//		break;
		//	case SurfaceType61:
		//		break;
		//	case SurfaceType62:
		//		break;
		//	case SurfaceType_Max:
		//		break;
		//	default:
		//		break;
		//	}
		//	
		//	// Spawn Projectile Ball
		//	
		//	

		//	/*TPVBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);*/
		///*	TPVBall->SphereComp->SetSimulatePhysics(true);
		//	TPVBall->SphereComp->AddImpulse(TraceEnd);*/

		//	/*if (DamageToApply > 0)
		//	{
		//		UGameplayStatics::ApplyPointDamage(HitActor, DamageToApply, CameraFowardVector, Hit, GetOwner()->GetInstigatorController(), TPVBall, TPVBall->DamageType);
		//	}*/

		//	DestroyBalls();
		//}
}

bool AxBaseCharacter::ServerThrowBall_Validate(FVector CameraLocation, FVector CameraFowardVector)
{
	return true;//IsValid(TPVBall) && !TPVBall->bIsExploding;
}

void AxBaseCharacter::ServerSetPLayerIsThrowing_Implementation(bool bPlayerIsThrowing)
{
	bIsThrowing = bPlayerIsThrowing;
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

	PlayerInputComponent->BindAction("ThrowBall", IE_Pressed, this, &AxBaseCharacter::IncreaseThrowPower);
	PlayerInputComponent->BindAction("ThrowBall", IE_Released, this, &AxBaseCharacter::PlayThrowBallAnim);

	PlayerInputComponent->BindAction("TryCatchBall", IE_Pressed, this, &AxBaseCharacter::ServerSetCatchMode);
	PlayerInputComponent->BindAction("CamToggle", IE_Pressed, this, &AxBaseCharacter::CamToggle);
}


void AxBaseCharacter::SpawnNewBallOnFPVMesh()
{
	FName SocketName = TEXT("hand_socket");

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform FPVSocketTransform = SkeletalMeshComp->GetSocketTransform(SocketName, RTS_World);

	FPVBall = SpawnBall(FPVSocketTransform);
	FPVBall->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPVBall->SetReplicates(false);
	FPVBall->SphereComp->SetOnlyOwnerSee(true);
	FPVBall->SetReplicateMovement(false);
	FPVBall->AttachToComponent(SkeletalMeshComp, TransformRules, SocketName);
}

void AxBaseCharacter::MulticastDestroyBalls_Implementation()
{

}

void AxBaseCharacter::DestroyBalls()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("DESTROY BALLS SERVER"));
	}
	ThrowPower = 0;
	bHasBall = false;
	bIsThrowing = false;
	bIsAddingThrowPower = false;
	/*GetWorld()->GetTimerManager().ClearAllTimersForObject(this);*/
	TPVBall->ClientStopWarn();
	//MulticastDestroyBalls();
	if (IsValid(FPVBall))
	{
		FPVBall->Destroy();
	}
	if (IsValid(TPVBall))
	{
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
	DOREPLIFETIME(AxBaseCharacter, ThrowPower);
	DOREPLIFETIME(AxBaseCharacter, MaxThrowPower);
	DOREPLIFETIME(AxBaseCharacter, Health);
	DOREPLIFETIME(AxBaseCharacter, bIsAddingThrowPower);
}

