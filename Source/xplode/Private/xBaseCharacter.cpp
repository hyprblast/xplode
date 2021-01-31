// Fill out your copyright notice in the Description page of Project Settings.


#include "xBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "xBallBase.h"

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

bool AxBaseCharacter::GetPlayerHasBall_Implementation()
{
	return bHasBall;
}

int32 AxBaseCharacter::SetPlayerHasBall_Implementation(bool bPlayerHasBall)
{
	bHasBall = bPlayerHasBall;
	return 1;
}

int32 AxBaseCharacter::AttachBall_Implementation(AxBallBase* Ball)
{
	if (Ball && Ball != nullptr)
	{
		// Have to attach the spawned ball to TPV mesh and spawn a new one for FPV mesh with only owner sees
		// TODO: will need some sort of sync component between the 2
		AttachBallToTPVMesh(Ball);
		SpawnNewBallOnFPVMesh();
	}

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
	UE_LOG(LogTemp, Log, TEXT("AttachBallToTPVMesh")); 
	FName SocketName = TEXT("hand_socket");
	USkeletalMeshComponent* TPVSkeletalMesh = GetMesh();
	
	/*Ball->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	/*Ball->SphereComp->SetGenerateOverlapEvents(false);*/
	Ball->SetOwner(this);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform TPVSocketTransform = TPVSkeletalMesh->GetSocketTransform(SocketName, RTS_World);

	Ball->SphereComp->SetOwnerNoSee(true);
	Ball->AttachToComponent(TPVSkeletalMesh, TransformRules, SocketName);

	/*Ball->SetActorTransform(TPVSocketTransform);*/
}


void AxBaseCharacter::SpawnNewBallOnFPVMesh()
{

	FName SocketName = TEXT("hand_socket");

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	FTransform FPVSocketTransform = SkeletalMeshComp->GetSocketTransform(SocketName, RTS_World);

	BallInHand = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), FPVSocketTransform, spawnParams);
	BallInHand->SphereComp->SetGenerateOverlapEvents(false);
	BallInHand->SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BallInHand->SphereComp->SetOnlyOwnerSee(true);
	BallInHand->SetReplicates(false);
	BallInHand->AttachToComponent(SkeletalMeshComp, TransformRules, SocketName);
	
}

// Called to bind functionality to input
void AxBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AxBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AxBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAxis("MoveFoward", this, &AxBaseCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AxBaseCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AxBaseCharacter::Jump);
}

void AxBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxBaseCharacter, bHasBall);
}

