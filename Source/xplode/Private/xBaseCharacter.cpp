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
#include "Engine/Blueprint.h"
#include "xplodeGameStateBase.h"
#include <Sound/SoundCue.h>
#include "PunchDamageType.h"
#include "UObject/UObjectGlobals.h"
#include "xBaseDamageTypeInterface.h"



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

	/*TPVMesh->SetCollisionObjectType(XBALLNOCOLLISIONOBJECT_CHANNEL);
	TPVMesh->SetCollisionProfileName(TEXT("xBalllMeshNoCollision"));*/
	TPVMesh->CanCharacterStepUp(false);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetWorldLocation(FVector(0, 0, 70.0f));
	/*CameraComp->bUsePawnControlRotation = false;*/
	CameraComp->SetupAttachment(CapsuleComp);
	CameraComp->SetIsReplicated(true);




	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	SkeletalMeshComp->CanCharacterStepUp(false);
	SkeletalMeshComp->SetupAttachment(CameraComp);


	/*SkeletalMeshComp->SetCollisionObjectType(XBALLNOCOLLISIONOBJECT_CHANNEL);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("xBalllMeshNoCollision"));*/
	SkeletalMeshComp->CanCharacterStepUp(false);

	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();

	MovementComponent->SetIsReplicated(true);
	MovementComponent->bUseControllerDesiredRotation = true;

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

	LeftHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollisionComp->SetSphereRadius(17.f);
	LeftHandCollisionComp->SetGenerateOverlapEvents(true);
	LeftHandCollisionComp->SetCollisionObjectType(XPUNCHOBJECT_CHANNEL);
	LeftHandCollisionComp->SetCollisionProfileName(TEXT("xPunchCollision"));
	LeftHandCollisionComp->CanCharacterStepUp(false);
	LeftHandCollisionComp->AttachToComponent(GetMesh(), TransformRules, TEXT("hand_l"));


	RightHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RighttHandCollision"));
	RightHandCollisionComp->SetSphereRadius(17.f);
	RightHandCollisionComp->SetGenerateOverlapEvents(true);
	RightHandCollisionComp->SetCollisionObjectType(XPUNCHOBJECT_CHANNEL);
	RightHandCollisionComp->SetCollisionProfileName(TEXT("xPunchCollision"));
	RightHandCollisionComp->CanCharacterStepUp(false);
	RightHandCollisionComp->AttachToComponent(GetMesh(), TransformRules, TEXT("hand_r"));


	/*GetCharacterMovement()->PushForceFactor =0;
	GetCharacterMovement()->InitialPushForceFactor = 0;
	GetCharacterMovement()->MaxTouchForce = 0;
	GetCharacterMovement()->RepulsionForce = 0;
	GetCapsuleComponent()->SetSimulatePhysics(false);*/

	Tags.Add(FName("Player"));

	bReplicates = true;
	SetReplicateMovement(true);


	/*static ConstructorHelpers::FObjectFinder<UBlueprint> GameCameraBP(TEXT("Blueprint'/Game/_Main/Actors/Blueprints/BP_GameCamera.BP_GameCamera'"));
	GameCamera = Cast<AxGameCamera>(GameCameraBP.Object);*/

}

void AxBaseCharacter::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor)
		&& OtherActor->ActorHasTag("Player")
		&& OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass())
		&& IxBaseCharacterInterface::Execute_GetPlayerType(OtherActor) != PlayerTypeName
		&& bIsPunching)
	{
		UPunchDamageType* PunchDamageType = NewObject<UPunchDamageType>();

		if (!IxBaseCharacterInterface::Execute_GetPlayerIsBlocking(OtherActor))
		{
			if (bIsLeftPunch)
			{
				IxBaseCharacterInterface::Execute_SetPlayerIsLeftHit(OtherActor, true);
				IxBaseCharacterInterface::Execute_SetPlayerIsRightHit(OtherActor, false);
			}
			else
			{
				IxBaseCharacterInterface::Execute_SetPlayerIsRightHit(OtherActor, true);
				IxBaseCharacterInterface::Execute_SetPlayerIsLeftHit(OtherActor, false);
			}

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("PUNCH"));
			UGameplayStatics::ApplyPointDamage(OtherActor, PunchDamageType->Damage, CameraComp->GetForwardVector(), SweepResult, GetOwner()->GetInstigatorController(), this, PunchDamageType->StaticClass());

		}
	}
}

bool AxBaseCharacter::GetPlayerIsThrowing_Implementation()
{
	return bIsThrowing;
}

FName AxBaseCharacter::GetPlayerType_Implementation()
{
	return PlayerTypeName;
}

bool AxBaseCharacter::GetPlayerIsBlocking_Implementation()
{
	return bIsBlocking;
}

bool AxBaseCharacter::GetPlayerHasBall_Implementation()
{
	return bHasBall;
}



bool AxBaseCharacter::GetPlayerIsDead_Implementation()
{
	return bIsDead;
}

int32 AxBaseCharacter::SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing)
{
	ServerSetPLayerIsThrowing(bPlayerIsThrowing);
	return 1;
}


int32 AxBaseCharacter::SetPlayerIsLeftHit_Implementation(bool bPlayerIsLeftHit)
{
	bIsLeftHit = bPlayerIsLeftHit;
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsPunching_Implementation(bool bPlayerIsThrowing)
{
	ServerSetPLayerIsPunching(bPlayerIsThrowing);
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsRightHit_Implementation(bool bPlayerIsRightHit)
{
	bIsRightHit = bPlayerIsRightHit;
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsGettingPunched_Implementation(bool bPlayerIsGettingPunched)
{
	ServerSetPLayerIsGettingPunched(bPlayerIsGettingPunched);
	return 1;
}

int32 AxBaseCharacter::ThrowBall_Implementation()
{
	/*const FVector FwVector (CameraComp->GetForwardVector().X, CameraComp->GetForwardVector().Y, CameraComp->GetForwardVector().Z);*/
	ServerThrowBall(CameraComp->GetForwardVector());
	return 1;
}

int32 AxBaseCharacter::PickupBall_Implementation(AxBallBase* Ball)
{
	TPVBall = Ball;
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
		SubscribeToBallWarnEvent();
		SubscribeToBallExplodeEvent();
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

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &AxBaseCharacter::HandleTakeAnyDamage);
		LeftHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxBaseCharacter::CallOnOverlap);
		RightHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxBaseCharacter::CallOnOverlap);
	}

	MulticastSetTopDownViewSettings();

	/*FindTopDownCamera();

	MulticastSetTopDownViewSettings();*/

}

//void AxBaseCharacter::Restart()
//{
//	//Called when the Pawn is being restarted(usually by being possessed by a Controller).
//
//	Super::Restart();
//	
//	/*if (IsLocallyControlled())
//	{
//		FindTopDownCamera();
//	}*/
//	
//	/*if (HasAuthority())
//	{
//		MulticastSetTopDownViewSettings();
//	}*/
//	
//}

//void AxBaseCharacter::PossessedBy(AController* NewController)
//{
//	//Called when this Pawn is possessed.Only called on the server(or in standalone).
//	Super::PossessedBy(NewController);
//
//	FindTopDownCamera();
//}

void AxBaseCharacter::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if (!bIsDead && IsValid(DamageType) && DamageType->GetClass()->ImplementsInterface(UxBaseDamageTypeInterface::StaticClass()))
	{
		bIsTakingHit = true;

		FName DamageTypeName = IxBaseDamageTypeInterface::Execute_GetDamageTypeName(DamageType);

		if (DamageTypeName == FName("Punch"))
		{
			if (bIsRightHit)
			{
				if (IsValid(GettingPunchedRightMontage))
				{
					MulticastPlayTPVGettingPunchedRightAnimation();

				}
			}
			else
			{
				if (IsValid(GettingPunchedLeftMontage))
				{
					MulticastPlayTPVGettingPunchedLeftAnimation();
				}
			}
			if (bHasBall)
			{
				if (bIsRightHit)
				{
					ServerThrowBall(CameraComp->GetRightVector());
				}
				else
				{
					ServerThrowBall(-CameraComp->GetRightVector());
				}
				
			}

		}

		Health -= Damage;

		if (Health <= 0 && !bIsDead)
		{
			bIsDead = true;
			MulticastKilled();
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(Health));
}

// Called every frame
void AxBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bIsAddingThrowPower && ThrowPower < MaxThrowPower)
		{
			ThrowPower += 500;

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


void AxBaseCharacter::FindTopDownCamera()
{

	TArray<AActor*> FoundCamActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AxGameCamera::StaticClass(), FoundCamActors);

	if (FoundCamActors.Num() == 2)
	{
		TopDownCam = PlayerTypeName == TEXT("Blue") ? Cast<AxGameCamera>(FoundCamActors[1]) : Cast<AxGameCamera>(FoundCamActors[0]);
	}
}


void AxBaseCharacter::AttachBallToTPVMesh()
{
	bHasBall = true;

	FName SocketName = TEXT("hand_socket");
	USkeletalMeshComponent* TPVSkeletalMesh = GetMesh();

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	FTransform TPVSocketTransform = TPVSkeletalMesh->GetSocketTransform(SocketName, RTS_World);

	TPVBall->AttachToComponent(TPVSkeletalMesh, TransformRules, SocketName);
	TPVBall->StartTimer();

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
	PunchLeftMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PunchLeftAnimMontage.PunchLeftAnimMontage'")));
	PunchRightMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PunchRightAnimMontage.PunchRightAnimMontage'")));
	GettingPunchedLeftMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/GettingPunchedLeftAnimMontage.GettingPunchedLeftAnimMontage'")));
	GettingPunchedRightMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/GettingPunchedRightAnimMontage.GettingPunchedRightAnimMontage'")));
	ThrowPowerIncreaseSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Charge/SCIMisc_Charge_Weapon_15_Cue.SCIMisc_Charge_Weapon_15_Cue'")));
	WarningSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Targeting/BEEP_Targeting_Loop_04_Cue.BEEP_Targeting_Loop_04_Cue'")));
	DieSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Sounds/SoundWaves/Death.Death'")));
}

void AxBaseCharacter::OnBallWarn()
{
	ClientPlayBallWarnEvent();
}

void AxBaseCharacter::OnBallExploding()
{
	ClientStopPlayBallWarn();
	MulticastKilledByExplosion();
}

void AxBaseCharacter::Turn(float Value)
{

	if (CameraVieww == UxCameraView::FirstPerson)
	{
		AxBaseCharacter::AddControllerYawInput(Value);
		InputAxisYawValue = Value;
	}
}

void AxBaseCharacter::PlayThrowBallAnim()
{

	if (bHasBall && !bIsThrowing && ThrowPower > 0)
	{
		bIsThrowing = true;
		SkeletalMeshComp->GetAnimInstance()->Montage_Play(ThrowBallMontage);
		ServerPlayTPVThrowBallAnim();
	}
}


void AxBaseCharacter::CamToggle()
{
	/*switch (CameraVieww)
	{
	case UxCameraView::FirstPerson:
		ServerSetTopDownViewSettings();
		break;
	case UxCameraView::TopDown:
		ServerSetFirstPersonViewSettings();
		break;
	default:
		break;
	}*/
}



void AxBaseCharacter::IncreaseThrowPower()
{
	if (bHasBall && !bIsThrowing)
	{
		if (!bIsAddingThrowPower)
		{
			ThrowPowerAudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ThrowPowerIncreaseSoundFx, GetActorLocation());
		}

		ServerIncreaseThrowPower();
	}
}

void AxBaseCharacter::PlayPunchLeftAnim()
{
	if (!bIsPunching)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(PunchLeftMontage);
		ServerPlayTPVPunchLeftAnim();
	}

	/*if (!SkeletalMeshComp->GetAnimInstance()->Montage_IsPlaying(PunchLeftMontage))
	{

	}*/

}

void AxBaseCharacter::PlayPunchRightAnim()
{

	if (!bIsPunching)
	{
		//TODO: Maybe if has ball needs other animation ???
		GetMesh()->GetAnimInstance()->Montage_Play(PunchRightMontage);
		ServerPlayTPVPunchRightAnim();
	}

	/*if (!SkeletalMeshComp->GetAnimInstance()->Montage_IsPlaying(PunchRightMontage))
	{

	}*/

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

void AxBaseCharacter::ServerSetFirstPersonViewSettings_Implementation()
{
	MulticastSetFirstPersonViewSettings();
}

bool AxBaseCharacter::ServerSetFirstPersonViewSettings_Validate()
{
	return true;
}

void AxBaseCharacter::ServerSetTopDownViewSettings_Implementation()
{
	MulticastSetTopDownViewSettings();
}


bool AxBaseCharacter::ServerSetTopDownViewSettings_Validate()
{
	return true;
}

void AxBaseCharacter::MulticastSetFirstPersonViewSettings_Implementation()
{
	CameraComp->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;


	if (HasAuthority())
	{
		SkeletalMeshComp->SetHiddenInGame(false);
		GetMesh()->bOwnerNoSee = true;

		ClientActivateFirstPersonViewCam();
	}
}

void AxBaseCharacter::MulticastSetTopDownViewSettings_Implementation()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
		MovementComponent->RotationRate = FRotator(0.0f, 840.0f, 0.0f);
		MovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = true;

	}



	if (HasAuthority())
	{
		SkeletalMeshComp->SetHiddenInGame(true);
		GetMesh()->bOwnerNoSee = false;
		ClientActivateTopDownViewCam();
	}
}

void AxBaseCharacter::MulticastKilledByExplosion_Implementation()
{
	if (CameraVieww == UxCameraView::TopDown)
	{
		// Ragdoll
		USkeletalMeshComponent* TPVMesh = GetMesh();
		TPVMesh->SetCollisionProfileName(TEXT("BlockAll"));
		TPVMesh->SetAllBodiesSimulatePhysics(true);
		TPVMesh->SetSimulatePhysics(true);
		TPVMesh->WakeAllRigidBodies();
		TPVMesh->bBlendPhysics = true;

		
		Die();
		
		

		/*TPVMesh->AddRadialForce(GetActorUpVector(), 50.f, 15000.f, ERadialImpulseFalloff::RIF_MAX);*/
	}
}

void AxBaseCharacter::MulticastKilled_Implementation()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DieSoundFx, GetActorLocation());
	Die();
}

void AxBaseCharacter::Die()
{
	SetLifeSpan(10.0f);
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->StopMovementImmediately();
	MovementComponent->DisableMovement();
	MovementComponent->SetComponentTickEnabled(false);
}


void AxBaseCharacter::ClientActivateTopDownViewCam_Implementation()
{
	if (IsLocallyControlled())
	{
		CameraVieww = UxCameraView::TopDown;

		if (!IsValid(TopDownCam))
		{
			FindTopDownCamera();
		}

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		OurPlayerController->SetViewTargetWithBlend(TopDownCam, .5f);

	}
}

void AxBaseCharacter::ClientSetPlayerTypeName_Implementation(FName TypeName)
{
	PlayerTypeName = TypeName;
}

void AxBaseCharacter::SubscribeToBallWarnEvent()
{
	TPVBall->OnWarning.AddDynamic(this, &AxBaseCharacter::OnBallWarn);
}

void AxBaseCharacter::UnSubscribeToBallWarnEvent()
{
	TPVBall->OnWarning.RemoveDynamic(this, &AxBaseCharacter::OnBallWarn);
}

void AxBaseCharacter::SubscribeToBallExplodeEvent()
{
	TPVBall->OnExploding.AddDynamic(this, &AxBaseCharacter::OnBallExploding);
}

void AxBaseCharacter::UnSubscribeToBallExplodeEvent()
{
	TPVBall->OnExploding.RemoveDynamic(this, &AxBaseCharacter::OnBallExploding);
}

void AxBaseCharacter::ClientPlayBallWarnEvent_Implementation()
{
	WarnAudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WarningSoundFx, GetActorLocation());
}


void AxBaseCharacter::ClientStopPlayBallWarn_Implementation()
{
	if (IsValid(WarnAudioComponent))
	{
		WarnAudioComponent->SetActive(false);
	}
}

void AxBaseCharacter::ClientActivateFirstPersonViewCam_Implementation()
{
	if (IsLocallyControlled())
	{
		CameraVieww = UxCameraView::FirstPerson;

		APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		OurPlayerController->SetViewTargetWithBlend(this, .5f);
	}
}

void AxBaseCharacter::ServerPlayTPVThrowBallAnim_Implementation()
{
	bIsAddingThrowPower = false;
	bIsThrowing = true;
	MulticastPlayTPVThrowAnimation();
}

bool AxBaseCharacter::ServerPlayTPVThrowBallAnim_Validate()
{
	return true;
}

void AxBaseCharacter::ServerPlayTPVPunchLeftAnim_Implementation()
{
	bIsPunching = true;
	bIsLeftPunch = true;
	MulticastPlayTPVPunchLeftAnimation();
}

bool AxBaseCharacter::ServerPlayTPVPunchLeftAnim_Validate()
{
	return true;
}

void AxBaseCharacter::ServerPlayTPVPunchRightAnim_Implementation()
{
	bIsPunching = true;
	bIsRightPunch = true;
	MulticastPlayTPVPunchRightAnimation();
}

bool AxBaseCharacter::ServerPlayTPVPunchRightAnim_Validate()
{
	return true;
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


void AxBaseCharacter::MulticastPlayTPVPunchLeftAnimation_Implementation()
{
	if (IsValid(PunchLeftMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(PunchLeftMontage);
	}
}

void AxBaseCharacter::MulticastPlayTPVPunchRightAnimation_Implementation()
{
	if (IsValid(PunchRightMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(PunchRightMontage);
	}
}

void AxBaseCharacter::MulticastPlayTPVGettingPunchedLeftAnimation_Implementation()
{
	if (IsValid(GettingPunchedLeftMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(GettingPunchedLeftMontage);
	}
}

void AxBaseCharacter::MulticastPlayTPVGettingPunchedRightAnimation_Implementation()
{
	if (IsValid(GettingPunchedRightMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(GettingPunchedRightMontage);
	}
}

void AxBaseCharacter::ServerThrowBall_Implementation(FVector CameraFowardVector)
{
	TPVBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	TPVBall->Shoot(CameraFowardVector * FMath::Clamp(ThrowPower, MinThrowPower, MaxThrowPower));
	UnSubscribeToBallWarnEvent();
	UnSubscribeToBallExplodeEvent();
	ClientStopPlayBallWarn();
	DestroyBalls();
}

bool AxBaseCharacter::ServerThrowBall_Validate(FVector CameraFowardVector)
{
	return true;
}

void AxBaseCharacter::ServerSetPLayerIsThrowing_Implementation(bool bPlayerIsThrowing)
{
	bIsThrowing = bPlayerIsThrowing;
}

bool AxBaseCharacter::ServerSetPLayerIsThrowing_Validate(bool bPlayerIsThrowing)
{
	return true;
}

void AxBaseCharacter::ServerSetPLayerIsPunching_Implementation(bool bPlayerIsPunching)
{
	bIsPunching = bPlayerIsPunching;
	bIsRightPunch = bPlayerIsPunching;
	bIsLeftPunch = bPlayerIsPunching;
}

bool AxBaseCharacter::ServerSetPLayerIsPunching_Validate(bool bPlayerIsPunching)
{
	return true;
}

void AxBaseCharacter::ServerSetPLayerIsGettingPunched_Implementation(bool bPlayerIsGettingPunched)
{
	bPlayerIsGettingPunched = bPlayerIsGettingPunched;
}

bool AxBaseCharacter::ServerSetPLayerIsGettingPunched_Validate(bool bPlayerIsGettingPunched)
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

	PlayerInputComponent->BindAction("PunchLeft", IE_Pressed, this, &AxBaseCharacter::PlayPunchLeftAnim);
	PlayerInputComponent->BindAction("PunchRight", IE_Pressed, this, &AxBaseCharacter::PlayPunchRightAnim);

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

void AxBaseCharacter::SetHasBallFalse()
{
	bHasBall = false;
	GetWorld()->GetTimerManager().ClearTimer(HasBallTimerHandle);
}

void AxBaseCharacter::DestroyBalls()
{
	ThrowPower = 0;
	bIsThrowing = false;
	bIsAddingThrowPower = false;

	GetWorldTimerManager().SetTimer(
		HasBallTimerHandle, this, &AxBaseCharacter::SetHasBallFalse, .3f, false);


	if (IsValid(FPVBall))
	{
		FPVBall->Destroy();
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
	DOREPLIFETIME(AxBaseCharacter, PlayerTypeName);
	DOREPLIFETIME(AxBaseCharacter, bIsPunching);
	DOREPLIFETIME(AxBaseCharacter, bIsBlocking);
	DOREPLIFETIME(AxBaseCharacter, bIsRightPunch);
	DOREPLIFETIME(AxBaseCharacter, bIsLeftPunch);
	DOREPLIFETIME(AxBaseCharacter, bIsTakingHit);
	DOREPLIFETIME(AxBaseCharacter, bIsLeftHit);
	DOREPLIFETIME(AxBaseCharacter, bIsRightHit);
	DOREPLIFETIME(AxBaseCharacter, bIsDead);


}

