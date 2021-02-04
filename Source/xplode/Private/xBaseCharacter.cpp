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

}

bool AxBaseCharacter::GetPlayerIsThrowing_Implementation()
{
	return bIsThrowing;
}

bool AxBaseCharacter::GetPlayerHasBall_Implementation()
{
	return bHasBall;
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

void AxBaseCharacter::ThrowBall()
{
	ServerThrowBall(CameraComp->GetComponentLocation(), CameraComp->GetForwardVector());
}

void AxBaseCharacter::ServerThrowBall_Implementation(FVector CameraLocation, FVector CameraFowardVector)
{
	FHitResult Hit;
	FTransform ThrowTo;
	FRotator LookAtRotation;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraFowardVector * 10000);
	
	bIsThrowing = true;

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, XBALTRACE_CHANNEL, QueryParams);
}

bool AxBaseCharacter::ServerThrowBall_Validate(FVector CameraLocation, FVector CameraFowardVector)
{
	return bHasBall && IsValid(TPVBall);
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
	PlayerInputComponent->BindAction("ThrowBall", IE_Pressed, this, &AxBaseCharacter::ThrowBall);
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
	/*FPVBall->SetReplicates(true);*/
	FPVBall->SetReplicateMovement(true);
	FPVBall->AttachToComponent(SkeletalMeshComp, TransformRules, SocketName);
	FPVBall->SetOwner(this);
}

void AxBaseCharacter::DestroyBalls()
{
	FPVBall = nullptr;
	TPVBall = nullptr;
	
	/*if (FPVBall && FPVBall != nullptr && )
	{
		UE_LOG(LogTemp, Log, TEXT("Destroy ball in hand"));
		FPVBall->Destroy();
	}*/
}

void AxBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxBaseCharacter, bHasBall);
	DOREPLIFETIME(AxBaseCharacter, FPVBall);
	DOREPLIFETIME(AxBaseCharacter, TPVBall);
	DOREPLIFETIME(AxBaseCharacter, bIsThrowing);
}

