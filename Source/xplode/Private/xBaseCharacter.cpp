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
#include "Sound/SoundCue.h"
#include "UObject/UObjectGlobals.h"
#include "xBaseDamageTypeInterface.h"
#include "Perception/AISenseConfig_Sight.h"
#include "FightDamageType.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "Math/UnrealMathUtility.h"
#include <Math/TransformNonVectorized.h>
//#include "PhysicsEngine/PhysicsHandleComponent.h"



// Sets default values
AxBaseCharacter::AxBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->SetRelativeLocation(DefaultSpringArmVector);
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = true;
	SpringArmComp->bInheritRoll = true;
	SpringArmComp->SetRelativeRotation(FRotator(DefaultSpringArmPitch, 0, 0));
	SpringArmComp->TargetArmLength = DefaultSpringArmLength;
	SpringArmComp->bDoCollisionTest = false;
	TargetSpringArmPitch = DefaultSpringArmPitch;
	TargetSpringArmLength = DefaultSpringArmLength;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->bUsePawnControlRotation = false;
	CameraComp->SetupAttachment(SpringArmComp);


	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->CanCharacterStepUp(false);

	//CapsuleComp->SetWalkableSlopeOverride()
	/*CapsuleComp->SetCollisionProfileName(TEXT("Spectator"));*/
	/*CapsuleComp->SetGenerateOverlapEvents(false);*/

	USkeletalMeshComponent* TPVMesh = GetMesh();
	TPVMesh->bReturnMaterialOnMove = true;

	TPVMesh->SetCollisionObjectType(XSKEKETALMESHOBJECT_CHANNEL);
	TPVMesh->SetCollisionProfileName(TEXT("xSkeletalMeshCollision"));
	TPVMesh->CanCharacterStepUp(false);
	TPVMesh->SetGenerateOverlapEvents(true);
	TPVMesh->CastShadow = false;

	//CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//CameraComp->SetWorldLocation(FVector(0, 0, 70.0f));
	///*CameraComp->bUsePawnControlRotation = false;*/
	//CameraComp->SetupAttachment(CapsuleComp);
	//CameraComp->SetIsReplicated(true);




	//SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	//SkeletalMeshComp->CanCharacterStepUp(false);
	//SkeletalMeshComp->bOnlyOwnerSee = true;
	//SkeletalMeshComp->SetupAttachment(CameraComp);



	/*SkeletalMeshComp->SetCollisionObjectType(XBALLNOCOLLISIONOBJECT_CHANNEL);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("xBalllMeshNoCollision"));*/
	//SkeletalMeshComp->CanCharacterStepUp(false);

	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();

	MovementComponent->SetIsReplicated(true);
	MovementComponent->bUseControllerDesiredRotation = true;
	/*MovementComponent->RotationRate = FRotator(0.0f, 180.0f, 0.0f);*/
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->MaxWalkSpeed = 1000.f;
	MovementComponent->SetWalkableFloorAngle(0);

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	LeftHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollisionComp->SetSphereRadius(17.f);
	LeftHandCollisionComp->SetGenerateOverlapEvents(true);
	LeftHandCollisionComp->SetCollisionObjectType(XPUNCHOBJECT_CHANNEL);
	LeftHandCollisionComp->SetCollisionProfileName(TEXT("xPunchCollision"));
	LeftHandCollisionComp->CanCharacterStepUp(false);
	LeftHandCollisionComp->AttachToComponent(GetMesh(), TransformRules, TEXT("hand_l"));
	LeftHandCollisionComp->bReturnMaterialOnMove = true;


	RightHandCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollisionComp->SetSphereRadius(17.f);
	RightHandCollisionComp->SetGenerateOverlapEvents(true);
	RightHandCollisionComp->SetCollisionObjectType(XPUNCHOBJECT_CHANNEL);
	RightHandCollisionComp->SetCollisionProfileName(TEXT("xPunchCollision"));
	RightHandCollisionComp->CanCharacterStepUp(false);
	RightHandCollisionComp->AttachToComponent(GetMesh(), TransformRules, TEXT("hand_r"));
	RightHandCollisionComp->bReturnMaterialOnMove = true;

	RightFootCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightFootCollision"));
	RightFootCollisionComp->SetRelativeLocation(FVector(8.7f, -15.f, 1));
	RightFootCollisionComp->SetSphereRadius(17.f);
	RightFootCollisionComp->SetGenerateOverlapEvents(true);
	RightFootCollisionComp->SetCollisionObjectType(XPUNCHOBJECT_CHANNEL);
	RightFootCollisionComp->SetCollisionProfileName(TEXT("xPunchCollision"));
	RightFootCollisionComp->CanCharacterStepUp(false);
	RightFootCollisionComp->AttachToComponent(GetMesh(), TransformRules, TEXT("ik_foot_r"));
	RightFootCollisionComp->bReturnMaterialOnMove = true;

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 3000.f;
	SightConfig->LoseSightRadius = 3500.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->SetMaxAge(10.f);

	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());


	/*PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));*/

	/*GetCharacterMovement()->PushForceFactor =0;
	GetCharacterMovement()->InitialPushForceFactor = 0;
	GetCharacterMovement()->MaxTouchForce = 0;
	GetCharacterMovement()->RepulsionForce = 0;
	GetCapsuleComponent()->SetSimulatePhysics(false);*/

	Tags.Add(FName("Player"));

	bReplicates = true;
	SetReplicateMovement(true);


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	/*static ConstructorHelpers::FObjectFinder<UBlueprint> GameCameraBP(TEXT("Blueprint'/Game/_Main/Actors/Blueprints/BP_GameCamera.BP_GameCamera'"));
	GameCamera = Cast<AxGameCamera>(GameCameraBP.Object);*/

}

void AxBaseCharacter::CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor)
		&& OtherActor->ActorHasTag("Player")
		&& OtherActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass())
		&& IxBaseCharacterInterface::Execute_GetPlayerType(OtherActor) != PlayerTypeName
		&& !IxBaseCharacterInterface::Execute_GetPlayerIsDead(OtherActor)
		&& !IxBaseCharacterInterface::Execute_GetPlayerIsKO(OtherActor)
		&& bIsFighting)
	{

		int32 ShouldAIBlock = FMath::RandRange(1, 50);

		if (!bIsSliding && (IxBaseCharacterInterface::Execute_GetPlayerIsBlocking(OtherActor) || (OtherActor->ActorHasTag("Bot") && ShouldAIBlock > 40)))
		{
			if (OtherActor->ActorHasTag("Bot"))
			{
				IxBaseCharacterInterface::Execute_AIBlock(OtherActor);
			}

			ClientPlayBlockSound();

			IxBaseCharacterInterface::Execute_PushPlayer(OtherActor, FVector(GetActorForwardVector().X * 100.f, GetActorForwardVector().Y * 100.f, 100.f), true, true);
			return;
		}

		/*	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(SweepResult.PhysMaterial.Get());

			switch (SurfaceType)
			{
			case HEAD:
			case TORSO:
			case ARMS:
			case LEGS:
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("HEAD"));
				A
				break;
			default:
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Default"));
				break;
			}*/

			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticles, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
		MulticastBloodCloud(OtherActor->GetActorLocation(), OtherActor->GetActorRotation());

		/*if (!GetCharacterMovement()->IsFalling())
		{
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OtherActor->GetActorLocation());
			MulticastSetActorRotation(LookAtRotation);
		}*/

		UFightDamageType* FightDamageType = NewObject<UFightDamageType>();

		if (!bIsSliding)
		{
			IxBaseCharacterInterface::Execute_PushPlayer(OtherActor, FVector(GetActorForwardVector().X * FightDamageType->DamageImpulse, GetActorForwardVector().Y * FightDamageType->DamageImpulse, 100.f), true, true);
		}
		else
		{
			IxBaseCharacterInterface::Execute_PushPlayer(OtherActor, FVector(GetActorForwardVector().X * FightDamageType->DamageImpulse * 2, GetActorForwardVector().Y * FightDamageType->DamageImpulse * 2, 200.f), true, true);
		}


		UGameplayStatics::ApplyPointDamage(OtherActor, FightDamageType->Damage, CameraComp->GetForwardVector(), SweepResult, GetOwner()->GetInstigatorController(), this, FightDamageType->StaticClass());

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

bool AxBaseCharacter::GetPlayerIsPickingUpBall_Implementation()
{
	return bIsPickingUpBall;
}

bool AxBaseCharacter::GetPlayerIsBlocking_Implementation()
{
	return bIsBlocking;
}

bool AxBaseCharacter::GetPlayerIsFighting_Implementation()
{
	return bIsFighting;
}

bool AxBaseCharacter::GetPlayerIsSliding_Implementation()
{
	return bIsSliding;
}

bool AxBaseCharacter::GetPlayerIsAutoFighting_Implementation()
{
	return bIsAutoFight;
}

bool AxBaseCharacter::GetPlayerIsGettingHit_Implementation()
{
	return bIsTakingHit;
}

bool AxBaseCharacter::GetPlayerHasBall_Implementation()
{
	return bHasBall;
}



uint8 AxBaseCharacter::SetPlayerHasBall_Implementation(bool PlayerHasBall)
{
	bHasBall = PlayerHasBall;
	return 1;
}

bool AxBaseCharacter::GetPlayerIsKO_Implementation()
{
	return bIsKO;
}

bool AxBaseCharacter::GetPlayerIsDead_Implementation()
{
	return bIsDead;
}

FGuid AxBaseCharacter::GetPlayerHittingMe_Implementation()
{
	return PlayerHittingMe;
}

FGuid AxBaseCharacter::GetPlayerId_Implementation()
{
	return PlayerId;
}

uint8 AxBaseCharacter::SetWeaponToSpawn_Implementation(AActor* Sender, TSubclassOf<AxWeaponBase> WeaponToSpawn)
{
	WeaponToBeSpawned = WeaponToSpawn;
	WeaponPickupChest = Sender;
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing)
{
	ServerSetPLayerIsThrowing(bPlayerIsThrowing);
	return 1;
}


int32 AxBaseCharacter::SetPlayerAutofight_Implementation(bool bPlayerAutofight)
{
	bIsAutoFight = bPlayerAutofight;
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsBLocking_Implementation(bool bPlayerIsBlocking)
{
	bIsBlocking = false;
	return 1;
}

uint8 AxBaseCharacter::AIBlock_Implementation()
{
	Block();
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsFighting_Implementation(bool bPlayerIsFighting)
{
	bIsFighting = bPlayerIsFighting;
	if (bIsSliding)
	{
		bIsSliding = false;
	}
	return 1;
}


int32 AxBaseCharacter::PushPlayer_Implementation(FVector Force, bool bXOverride, bool bZOverride)
{
	LaunchCharacter(Force, bXOverride, bZOverride);
	return 1;
}

int32 AxBaseCharacter::SetPlayerIsGettingHit_Implementation(bool bPlayerIsGettingHit)
{
	if (!ActorHasTag("Bot"))
	{
		bPlayerIsGettingHit = bPlayerIsGettingHit;
	}

	bIsKO = false;

	return 1;
}

int32 AxBaseCharacter::ThrowBall_Implementation()
{
	/*const FVector FwVector (CameraComp->GetForwardVector().X, CameraComp->GetForwardVector().Y, CameraComp->GetForwardVector().Z);*/
	//ClientThrowBall(GetActorForwardVector(), false);
	ServerThrowBall(GetActorForwardVector(), false);
	return 1;
}

int32 AxBaseCharacter::PickupBall_Implementation(AxBallBase* Ball)
{
	bIsPickingUpBall = true;
	TPVBall = Ball;
	MulticastPlayTPVPickupAnimation();
	return 1;
}

float AxBaseCharacter::GetInputAxisYawValue_Implementation()
{
	return InputAxisYawValue;
}

int32 AxBaseCharacter::AttachBall_Implementation()
{
	if (HasAuthority())
	{
		bHasBall = true;
		bIsPickingUpBall = false;
		AttachBallToTPVMesh();
		SubscribeToBallWarnEvent();
		SubscribeToBallExplodeEvent();
	}
	/*else if (IsLocallyControlled())
	{
		SpawnNewBallOnFPVMesh();
	}*/

	return 1;
}

// Called when the game starts or when spawned
void AxBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadDynamicRefs();

	if (HasAuthority())
	{

		/*	PhysicsHandle->SetActive(false);*/

			// Create unique id for this player
		PlayerId = FGuid::NewGuid();

		OnTakeAnyDamage.AddDynamic(this, &AxBaseCharacter::HandleTakeAnyDamage);
		LeftHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxBaseCharacter::CallOnOverlap);
		RightHandCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxBaseCharacter::CallOnOverlap);
		RightFootCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AxBaseCharacter::CallOnOverlap);

		if (ActorHasTag("Bot"))
		{
			AAIController* AIController = UAIBlueprintHelperLibrary::GetAIController(this);

			if (IsValid(AIController) && IsValid(BehaviourTree))
			{
				AIController->RunBehaviorTree(BehaviourTree);
			}
		}
	}

	/*MulticastSetTopDownViewSettings();*/

	/*FindTopDownCamera();

	MulticastSetTopDownViewSettings();*/

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BackgroundSoundFx, GetActorLocation());
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

	if (IsValid(DamageType) && DamageType->GetClass()->ImplementsInterface(UxBaseDamageTypeInterface::StaticClass()))
	{
		FName DamageTypeName = IxBaseDamageTypeInterface::Execute_GetDamageTypeName(DamageType);

		PlayerHittingMe = IxBaseCharacterInterface::Execute_GetPlayerId(DamageCauser);

		bool isSlide = IxBaseCharacterInterface::Execute_GetPlayerIsSliding(DamageCauser);
		bIsKO = isSlide;


		if (DamageTypeName == TEXT("Fight"))
		{
			int32 GettingPunchedMoveIndex = FMath::RandRange(0, GettingPunchedMontages.Num() - 1);


			bIsTakingHit = true;

			MulticastGettingPunchedLogic(GettingPunchedMoveIndex, isSlide);

			if (bHasBall)
			{
				ServerThrowBall(-CameraComp->GetForwardVector(), true);
			}
		}

		Health -= Damage;

		if (ActorHasTag("Bot"))
		{
			bIsAutoFight = true;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(Health));
}

// Called every frame
void AxBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		if (TargetSpringArmPitch != SpringArmComp->GetRelativeRotation().Pitch)
		{
			ChangeSpringArmPitch(DeltaTime);
		}

		if (TargetSpringArmLength != SpringArmComp->TargetArmLength)
		{
			ChangeSpringArmLength(DeltaTime);
		}

	}

	if (HasAuthority())
	{
		if (!bIsDead)
		{
			/*if (PhysicsHandle->IsActive())
			{
				FTransform TPVSocketTransform = GetMesh()->GetSocketTransform(TEXT("hand_socket"), RTS_World);
				PhysicsHandle->SetTargetLocationAndRotation(TPVSocketTransform.GetLocation(), TPVSocketTransform.GetRotation().Rotator());
			}*/


			if (bIsAddingThrowPower && ThrowPower < MaxThrowPower)
			{
				ThrowPower += 500;

				/*if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::SanitizeFloat(ThrowPower));
				}*/
			}
			if (Health <= 0 && !bIsDead)
			{
				bIsDead = true;
				bHasBall = false;

				if (!bIsKO)
				{
					MulticastKilled();
				}
			}
			if (ActorHasTag("Bot"))
			{

				/*Setting attack mode for bot on the server
				2 attack conditions - 1: Autofight (this happens when is getting hit or when is going after the ball, this last one is set on BT) and 2: it has ball possession.*/

				if (!bIsKO && (bIsAutoFight || bHasBall))
				{
					if (!bIsAttackMode)
					{
						bIsAttackMode = true;
						int32 Time = FMath::RandRange(1.f, 5.f);

						FTimerHandle TimerHandle;

						GetWorldTimerManager().SetTimer(
							TimerHandle, this, &AxBaseCharacter::AIDisableAttackMode, Time, false);


						GetWorldTimerManager().SetTimer(
							AIDisableAttackModeTimerHandle, this, &AxBaseCharacter::AITraceAndAttack, .5f, true);
					}
				}
				else
				{
					if (bIsAttackMode)
					{
						bIsAttackMode = false;
					}
				}

			}
		}
	}
}

void AxBaseCharacter::MoveFoward(float Value)
{
	if (bIsKO || bIsDead || bIsFighting || bIsSliding || bIsBlocking)
	{
		return;
	}

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction * Value);
}

void AxBaseCharacter::MoveRight(float Value)
{
	if (bIsKO || bIsDead || bIsFighting || bIsSliding || bIsBlocking)
	{
		return;
	}

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
	FName SocketName = TEXT("hand_socket");
	USkeletalMeshComponent* TPVSkeletalMesh = GetMesh();





	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform TPVSocketTransform = TPVSkeletalMesh->GetSocketTransform(SocketName, RTS_World);
	/*PhysicsHandle->SetActive(true);
	PhysicsHandle->GrabComponentAtLocationWithRotation(TPVBall->SphereComp, SocketName, TPVSocketTransform.GetLocation(), TPVSocketTransform.GetRotation().Rotator());
	*/
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
	/*ThrowBallMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/ThrowBallAnimMontage.ThrowBallAnimMontage'")));*/
	ThrowBallMontageTPV = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/ThrowBallAnimMontage.ThrowBallAnimMontage'")));
	PickupBallMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PickUpAnimMontage.PickUpAnimMontage'")));

	UAnimMontage* PunchLeftMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PunchLeftAnimMontage.PunchLeftAnimMontage'")));
	UAnimMontage* PunchRightMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PunchRightAnimMontage.PunchRightAnimMontage'")));
	UAnimMontage* OverhandLeftMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PunchLeftOverheadAnimMontage.PunchLeftOverheadAnimMontage'")));
	UAnimMontage* OverhandRightMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/PunchRightOverheadAnimMontage.PunchRightOverheadAnimMontage'")));
	UAnimMontage* UpperCutLeftMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/UpperCutPunchLeftAnimMontage.UpperCutPunchLeftAnimMontage'")));
	UAnimMontage* UpperCutRightMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/UpperCutPunchRightAnimMontage.UpperCutPunchRightAnimMontage'")));
	UAnimMontage* KickMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/KickAnimMontage.KickAnimMontage'")));
	UAnimMontage* RoundKickMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/RoundKickAnimMontage.RoundKickAnimMontage'")));
	/*UAnimMontage* HardKickMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/HardKickAnimMontage.HardKickAnimMontage'")));*/
	UAnimMontage* RotatingPunchMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/RotatingPunchAnimMontage.RotatingPunchAnimMontage'")));

	FightAnimMontages.Add(PunchLeftMontage);
	FightAnimMontages.Add(PunchRightMontage);
	FightAnimMontages.Add(OverhandLeftMontage);
	FightAnimMontages.Add(OverhandRightMontage);
	FightAnimMontages.Add(UpperCutLeftMontage);
	FightAnimMontages.Add(UpperCutRightMontage);
	/*FightAnimMontages.Add(RotatingPunchMontage);*/


	/*FightKickAnimMontages.Add(KickMontage);*/
	FightKickAnimMontages.Add(RoundKickMontage);
	/*FightKickAnimMontages.Add(HardKickMontage);*/


	UAnimMontage* GettingPunchedLeftMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/GettingPunchedLeftAnimMontage.GettingPunchedLeftAnimMontage'")));
	UAnimMontage* GettingPunchedRightMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/GettingPunchedRightAnimMontage.GettingPunchedRightAnimMontage'")));
	GettingPunchedMontages.Add(GettingPunchedLeftMontage);
	GettingPunchedMontages.Add(GettingPunchedRightMontage);

	GettingHitFromSlideMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/GettingHitFromSlideAnimMontage.GettingHitFromSlideAnimMontage'")));
	SlideMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/SlideAnimMontage.SlideAnimMontage'")));
	BlockMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), NULL, TEXT("AnimMontage'/Game/_Main/Characters/Animations/Montages/BlockAnimMontage.BlockAnimMontage'")));
	ThrowPowerIncreaseSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Charge/SCIMisc_Charge_Weapon_15_Cue.SCIMisc_Charge_Weapon_15_Cue'")));
	WarningSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/SciFiWeaponsCyberpunkArsenal/cues/Support_Material/Targeting/BEEP_Targeting_Loop_04_Cue.BEEP_Targeting_Loop_04_Cue'")));
	BlockSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/_Main/SFX/Block_Cue.Block_Cue'")));
	DieSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Sounds/SoundCues/Twinblast_Effort_Death.Twinblast_Effort_Death'")));
	BackgroundSoundFx = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("SoundCue'/Game/_Main/SFX/background_Cue.background_Cue'")));

	BehaviourTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), NULL, TEXT("BehaviorTree'/Game/_Main/Misc/AI/BT_xBot.BT_xBot'")));

	BloodParticles = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Impacts/DynamicImpacts/Flesh/Blood_cloud_Dyn_2.Blood_cloud_Dyn_2'")));

	/*FightTrailParticle = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, TEXT("ParticleSystem'/Game/BallisticsVFX/Particles/Explosive/Whispy_Trail.Whispy_Trail'")));*/
}

void AxBaseCharacter::ChangeSpringArmPitch(float DeltaTime)
{

	SpringArmComp->SetRelativeRotation(FMath::RInterpTo(SpringArmComp->GetRelativeRotation(), FRotator(TargetSpringArmPitch, 0, 0), DeltaTime, 2.f));
}

void AxBaseCharacter::ChangeSpringArmLength(float DeltaTime)
{
	float CurrentLength = SpringArmComp->TargetArmLength;
	SpringArmComp->TargetArmLength = FMath::FInterpTo(CurrentLength, TargetSpringArmLength, DeltaTime, 2.f);
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

void AxBaseCharacter::ChangeCameraPitch(float Value)
{
	TargetSpringArmPitch += Value;
	TargetSpringArmPitch = FMath::Clamp(TargetSpringArmPitch, DefaultSpringArmPitch, -10.f);

	TargetSpringArmLength -= 100.f * Value;
	TargetSpringArmLength = FMath::Clamp(TargetSpringArmLength, 300.f, DefaultSpringArmLength);
}

void AxBaseCharacter::PlayThrowBallAnim()
{


	if (bHasBall && !bIsThrowing && ThrowPower > 0)
	{
		bIsThrowing = true;
		//SkeletalMeshComp->GetAnimInstance()->Montage_Play(ThrowBallMontage);
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



void AxBaseCharacter::SpawnWweapon()
{
	if (WeaponToBeSpawned != NULL)
	{
		ServerSpawnWweapon();
	}
}

void AxBaseCharacter::ServerSpawnWweapon_Implementation()
{
	MulticastSpawnWweapon();
	WeaponPickupChest->Destroy();
}

bool AxBaseCharacter::ServerSpawnWweapon_Validate()
{
	return true;
}

void AxBaseCharacter::MulticastSpawnWweapon_Implementation()
{
	FTransform SocketTransform;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	MyWeapon = GetWorld()->SpawnActor<AxWeaponBase>(WeaponToBeSpawned, SocketTransform, spawnParams);

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	MyWeapon->SetActorTransform(GetMesh()->GetSocketTransform(TEXT("weaponSocket"), RTS_World));
	MyWeapon->AttachToComponent(GetMesh(), TransformRules, TEXT("weaponSocket"));

	WeaponToBeSpawned = NULL;
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

void AxBaseCharacter::Kick()
{
	if (!AreFightingRelatedMontagesPlaying())
	{
		ServerFight(true, 0);
		KickLogic(0);
	}

}

void AxBaseCharacter::Slide()
{
	if (!AreFightingRelatedMontagesPlaying())
	{
		ServerSlide();
		SlideLogic();
	}

}

void AxBaseCharacter::Fight()
{
	if (!AreFightingRelatedMontagesPlaying())
	{
		int32 FightMoveIndex = FMath::RandRange(0, FightAnimMontages.Num() - 1);

		ServerFight(false, FightMoveIndex);
		FightLogic(FightMoveIndex);
	}


}

void AxBaseCharacter::Block()
{
	if (!AreFightingRelatedMontagesPlaying())
	{
		ServerBlock();
		BlockLogic();

	}
}


bool AxBaseCharacter::AreFightingRelatedMontagesPlaying()
{
	return GetMesh()->GetAnimInstance()->Montage_IsPlaying(BlockMontage)
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(SlideMontage)
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(GettingHitFromSlideMontage)
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(GettingPunchedMontages[0]) || GetMesh()->GetAnimInstance()->Montage_IsPlaying(GettingPunchedMontages[1])
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightAnimMontages[0]) || GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightAnimMontages[1])
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightAnimMontages[2]) || GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightAnimMontages[3])
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightAnimMontages[4]) || GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightAnimMontages[5])
		|| GetMesh()->GetAnimInstance()->Montage_IsPlaying(FightKickAnimMontages[0]);
}

void AxBaseCharacter::BlockMontageOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsBlocking = false;
	}

}

void AxBaseCharacter::BlockMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsBlocking = false;
	}
}

void AxBaseCharacter::SlideMontageOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsSliding = false;
		bIsFighting = false;
	}
}

void AxBaseCharacter::SlideMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsSliding = false;
		bIsFighting = false;
	}
}

void AxBaseCharacter::GettingHitFromSlideMontageOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsKO = false;
		bIsTakingHit = false;
	}
}

void AxBaseCharacter::GettingHitFromSlideMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsKO = false;
		bIsTakingHit = false;
	}
}

void AxBaseCharacter::GettingPunchedMontageOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsTakingHit = false;
	}
}

void AxBaseCharacter::GettingPunchedMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsTakingHit = false;
	}
}

void AxBaseCharacter::FightMontageOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsFighting = false;
	}
}

void AxBaseCharacter::FightMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bIsFighting = false;
	}
}

void AxBaseCharacter::ThrowBallMontegaOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bHasBall = false;
	}
}

void AxBaseCharacter::ThrowBallMontegaOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bHasBall = false;
	}
}

void AxBaseCharacter::PickupBallMontageOnAnimationBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bHasBall = true;
		bIsPickingUpBall = false;
	}
}

void AxBaseCharacter::PickupBallMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	if (bInterrupted)
	{
		bHasBall = true;
		bIsPickingUpBall = false;
	}
}

void AxBaseCharacter::BlockLogic()
{
	bIsBlocking = true;
	GetMesh()->GetAnimInstance()->Montage_Play(BlockMontage);
	FOnMontageEnded BlendOutDelegate;
	BlendOutDelegate.BindUObject(this, &AxBaseCharacter::BlockMontageOnAnimationBlendOut);
	GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, BlockMontage);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AxBaseCharacter::BlockMontageOnAnimationEnd);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, BlockMontage);
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
	//CameraComp->bUsePawnControlRotation = true;
	//bUseControllerRotationYaw = true;
	//GetCharacterMovement()->bOrientRotationToMovement = false;


	//if (HasAuthority())
	//{
	///*	SkeletalMeshComp->SetHiddenInGame(false);*/
	//	GetMesh()->bOwnerNoSee = true;

	//	ClientActivateFirstPersonViewCam();
	//}
}

void AxBaseCharacter::MulticastSetActorRotation_Implementation(FRotator TargetRotation)
{
	/*SetActorRotation(TargetRotation);*/
}

void AxBaseCharacter::MulticastSetTopDownViewSettings_Implementation()
{
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;

	//UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();

	//if (MovementComponent)
	//{
	//	MovementComponent->bOrientRotationToMovement = true;
	//	MovementComponent->bUseControllerDesiredRotation = false;
	//	MovementComponent->RotationRate = FRotator(0.0f, 840.0f, 0.0f);
	//	MovementComponent->bAllowPhysicsRotationDuringAnimRootMotion = true;

	//}



	//if (HasAuthority())
	//{
	//	/*SkeletalMeshComp->SetHiddenInGame(true);*/
	//	GetMesh()->bOwnerNoSee = false;
	//	ClientActivateTopDownViewCam();
	//}
}

void AxBaseCharacter::MulticastKilledByExplosion_Implementation()
{
	// Ragdoll
	USkeletalMeshComponent* TPVMesh = GetMesh();
	TPVMesh->SetCollisionProfileName(TEXT("BlockAll"));
	TPVMesh->SetAllBodiesSimulatePhysics(true);
	TPVMesh->SetSimulatePhysics(true);
	TPVMesh->WakeAllRigidBodies();
	TPVMesh->bBlendPhysics = true;


	Die();
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


void AxBaseCharacter::FightLogic(uint8 FightMoveIndex)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		FVector FowardVector = GetActorForwardVector();
		PushPlayer(FVector(FowardVector.X * 300.f, FowardVector.Y * 300.f, 0), false, false);
	}

	GetMesh()->GetAnimInstance()->Montage_Play(FightAnimMontages[FightMoveIndex]);

	FOnMontageEnded BlendOutDelegate;
	BlendOutDelegate.BindUObject(this, &AxBaseCharacter::FightMontageOnAnimationBlendOut);
	GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, FightAnimMontages[FightMoveIndex]);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AxBaseCharacter::FightMontageOnAnimationEnd);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, FightAnimMontages[FightMoveIndex]);
}

void AxBaseCharacter::KickLogic(uint8 FightMoveIndex)
{

	if (!GetCharacterMovement()->IsFalling())
	{
		FVector FowardVector = GetActorForwardVector();
		PushPlayer(FVector(FowardVector.X * 500.f, FowardVector.Y * 500.f, 0), false, false);
	}

	GetMesh()->GetAnimInstance()->Montage_Play(FightKickAnimMontages[FightMoveIndex]);

	FOnMontageEnded BlendOutDelegate;
	BlendOutDelegate.BindUObject(this, &AxBaseCharacter::FightMontageOnAnimationBlendOut);
	GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, FightKickAnimMontages[FightMoveIndex]);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AxBaseCharacter::FightMontageOnAnimationEnd);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, FightKickAnimMontages[FightMoveIndex]);



}


void AxBaseCharacter::SlideLogic()
{

	GetMesh()->GetAnimInstance()->Montage_Play(SlideMontage);
	FOnMontageEnded BlendOutDelegate;
	BlendOutDelegate.BindUObject(this, &AxBaseCharacter::SlideMontageOnAnimationBlendOut);
	GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, SlideMontage);

	FOnMontageEnded CompleteDelegate;
	CompleteDelegate.BindUObject(this, &AxBaseCharacter::SlideMontageOnAnimationEnd);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, SlideMontage);

	if (!GetCharacterMovement()->IsFalling())
	{
		FVector FowardVector = GetActorForwardVector();
		PushPlayer(FVector(FowardVector.X * 700.f, FowardVector.Y * 700.f, 300.f), true, false);
	}
	
}

void AxBaseCharacter::AITraceAndAttack()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + (GetActorForwardVector() * 250);

	/*DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255.0, 0, 0), true);*/
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();

		if (IsValid(HitActor) &&
			HitActor->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
			(bIsAutoFight ? PlayerHittingMe == IxBaseCharacterInterface::Execute_GetPlayerId(HitActor) : true) &&
			!IxBaseCharacterInterface::Execute_GetPlayerIsDead(HitActor))
		{

			if (CurrentFightMoveIndex > FightAnimMontages.Num())
			{
				CurrentFightMoveIndex = 0;
			}

			if (!AreFightingRelatedMontagesPlaying())
			{
				if (CurrentFightMoveIndex <= FightAnimMontages.Num() - 1)
				{
					ServerFight(false, CurrentFightMoveIndex);
				}
				else
				{
					int32 ShouldAISlide = FMath::RandRange(1, 10);

					if (ShouldAISlide % 2 == 0)
					{
						ServerSlide();
					}
					else
					{
						ServerFight(true, 0);
					}
				}

				CurrentFightMoveIndex++;
			}
		}
	}
}


void AxBaseCharacter::SetHasBallFalse()
{
	bHasBall = false;
}

void AxBaseCharacter::AIDisableAttackMode()
{
	bIsAttackMode = false;
	bIsAutoFight = false;
	bIsTakingHit = false;
	bIsFighting = false;
	bIsBlocking = false;
	GetWorld()->GetTimerManager().ClearTimer(AIDisableAttackModeTimerHandle);
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

void AxBaseCharacter::ServerFight_Implementation(bool bIsKick, uint8 FightMoveIndex)
{
	bIsFighting = true;
	MulticastFight(bIsKick, FightMoveIndex);


}

bool AxBaseCharacter::ServerFight_Validate(bool bIsKick, uint8 FightMoveIndex)
{
	return true;
}

void AxBaseCharacter::ServerSlide_Implementation()
{
	bIsFighting = true;
	bIsSliding = true;
	MulticastSlide();
}

bool AxBaseCharacter::ServerSlide_Validate()
{
	return true;
}

void AxBaseCharacter::MulticastPlayTPVPickupAnimation_Implementation()
{
	if (IsValid(PickupBallMontage) && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(PickupBallMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(PickupBallMontage);

		FOnMontageEnded BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &AxBaseCharacter::PickupBallMontageOnAnimationBlendOut);
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, PickupBallMontage);

		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &AxBaseCharacter::PickupBallMontageOnAnimationEnd);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, PickupBallMontage);
	}
}



void AxBaseCharacter::ServerBlock_Implementation()
{
	bIsBlocking = true;
	MulticastBlock();
}

bool AxBaseCharacter::ServerBlock_Validate()
{
	return true;
}


void AxBaseCharacter::MulticastBlock_Implementation()
{

	if (!IsLocallyControlled())
	{
		BlockLogic();
	}
}

void AxBaseCharacter::MulticastFight_Implementation(bool bIsKick, uint8 FightMoveIndex)
{
	if (!IsLocallyControlled() || bIsAutoFight)
	{
		if (!bIsKick)
		{
			FightLogic(FightMoveIndex);
		}
		else
		{
			KickLogic(FightMoveIndex);
		}

	}

}

void AxBaseCharacter::MulticastSlide_Implementation()
{
	if (!IsLocallyControlled() || bIsAutoFight)
	{
		SlideLogic();
	}
}

void AxBaseCharacter::MulticastGettingPunchedLogic_Implementation(uint8 GettingPunchedMoveIndex, bool isSlide)
{

	if (!isSlide && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(GettingPunchedMontages[GettingPunchedMoveIndex]))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(GettingPunchedMontages[GettingPunchedMoveIndex]);

		FOnMontageEnded BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &AxBaseCharacter::GettingPunchedMontageOnAnimationBlendOut);
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, GettingPunchedMontages[GettingPunchedMoveIndex]);

		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &AxBaseCharacter::GettingPunchedMontageOnAnimationEnd);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, GettingPunchedMontages[GettingPunchedMoveIndex]);
	}
	else if (isSlide && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(GettingHitFromSlideMontage))
	{
		GetMesh()->GetAnimInstance()->Montage_Play(GettingHitFromSlideMontage);

		FOnMontageEnded BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &AxBaseCharacter::GettingHitFromSlideMontageOnAnimationBlendOut);
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, GettingHitFromSlideMontage);

		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &AxBaseCharacter::GettingHitFromSlideMontageOnAnimationEnd);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, GettingHitFromSlideMontage);

	}




}

void AxBaseCharacter::ServerThrowBall_Implementation(FVector CameraFowardVector, bool bJustDropBall)
{
	TPVBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	TPVBall->Shoot(CameraFowardVector * (!bJustDropBall ? FMath::Clamp(ThrowPower, MinThrowPower, MaxThrowPower) : 5000.f));
	UnSubscribeToBallWarnEvent();
	UnSubscribeToBallExplodeEvent();
	ClientStopPlayBallWarn();
	ThrowPower = 0;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &AxBaseCharacter::SetHasBallFalse, .3f, false);

}

bool AxBaseCharacter::ServerThrowBall_Validate(FVector CameraFowardVector, bool bJustDropBall)
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


void AxBaseCharacter::MulticastPlayTPVThrowAnimation_Implementation()
{
	// Play on all instances of this actor
	if (IsValid(ThrowBallMontageTPV))
	{
		bIsThrowing = true;
		GetMesh()->GetAnimInstance()->Montage_Play(ThrowBallMontageTPV);

		FOnMontageEnded BlendOutDelegate;
		BlendOutDelegate.BindUObject(this, &AxBaseCharacter::ThrowBallMontegaOnAnimationBlendOut);
		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(BlendOutDelegate, ThrowBallMontageTPV);

		FOnMontageEnded CompleteDelegate;
		CompleteDelegate.BindUObject(this, &AxBaseCharacter::ThrowBallMontegaOnAnimationEnd);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, ThrowBallMontageTPV);
	}
}

void AxBaseCharacter::ClientPlayBlockSound_Implementation()
{
	if (IsValid(BlockSoundFx))
	{
		if (IsValid(BlockAudioComponent))
		{
			BlockAudioComponent->SetActive(false);
		}

		BlockAudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BlockSoundFx, GetActorLocation());
	}
}

void AxBaseCharacter::MulticastBloodCloud_Implementation(FVector ImpactPoint, FRotator Rotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticles, ImpactPoint, Rotation);
}

// Called to bind functionality to input
void AxBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*PlayerInputComponent->BindAxis("LookUp", this, &AxBaseCharacter::AddControllerPitchInput);*/
	PlayerInputComponent->BindAxis("LookUp", this, &AxBaseCharacter::ChangeCameraPitch);
	PlayerInputComponent->BindAxis("Turn", this, &AxBaseCharacter::Turn);

	PlayerInputComponent->BindAxis("MoveFoward", this, &AxBaseCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AxBaseCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AxBaseCharacter::Jump);

	PlayerInputComponent->BindAction("ThrowBall", IE_Pressed, this, &AxBaseCharacter::IncreaseThrowPower);
	PlayerInputComponent->BindAction("ThrowBall", IE_Released, this, &AxBaseCharacter::PlayThrowBallAnim);

	PlayerInputComponent->BindAction("Fight", IE_Pressed, this, &AxBaseCharacter::Fight);

	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AxBaseCharacter::Kick);
	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AxBaseCharacter::Slide);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AxBaseCharacter::Block);

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AxBaseCharacter::SpawnWweapon);

	PlayerInputComponent->BindAction("CamToggle", IE_Pressed, this, &AxBaseCharacter::CamToggle);
}


void AxBaseCharacter::SpawnNewBallOnFPVMesh()
{
	//FName SocketName = TEXT("hand_socket");

	//FActorSpawnParameters spawnParams;
	//spawnParams.Owner = this;
	//spawnParams.Instigator = this;

	//FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	//FTransform FPVSocketTransform = SkeletalMeshComp->GetSocketTransform(SocketName, RTS_World);

	//FPVBall = SpawnBall(FPVSocketTransform);
	//FPVBall->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//FPVBall->SetReplicates(false);
	//FPVBall->SphereComp->SetOnlyOwnerSee(true);
	//FPVBall->SetReplicateMovement(false);
	//FPVBall->AttachToComponent(SkeletalMeshComp, TransformRules, SocketName);
}


void AxBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxBaseCharacter, bHasBall);
	DOREPLIFETIME(AxBaseCharacter, FPVBall);
	DOREPLIFETIME(AxBaseCharacter, TPVBall);
	DOREPLIFETIME(AxBaseCharacter, bIsThrowing);
	DOREPLIFETIME(AxBaseCharacter, bIsBlocking);
	DOREPLIFETIME(AxBaseCharacter, bIsFighting);
	DOREPLIFETIME(AxBaseCharacter, bIsTakingHit);
	DOREPLIFETIME(AxBaseCharacter, ThrowPower);
	DOREPLIFETIME(AxBaseCharacter, MaxThrowPower);
	DOREPLIFETIME(AxBaseCharacter, Health);
	DOREPLIFETIME(AxBaseCharacter, bIsAddingThrowPower);
	DOREPLIFETIME(AxBaseCharacter, PlayerTypeName);
	DOREPLIFETIME(AxBaseCharacter, bIsDead);
	DOREPLIFETIME(AxBaseCharacter, PlayerId);
	DOREPLIFETIME(AxBaseCharacter, PlayerHittingMe);
	DOREPLIFETIME(AxBaseCharacter, bIsAutoFight);
	DOREPLIFETIME(AxBaseCharacter, bIsKO);
	DOREPLIFETIME(AxBaseCharacter, bIsAttackMode);
	DOREPLIFETIME(AxBaseCharacter, bIsSliding);
	DOREPLIFETIME(AxBaseCharacter, bIsPickingUpBall);
	DOREPLIFETIME(AxBaseCharacter, WeaponToBeSpawned);


}

