// Fill out your copyright notice in the Description page of Project Settings.


#include "xBaseCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AxBaseCharacter::AxBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UCapsuleComponent* CapsuleComp = this->GetCapsuleComponent();

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetWorldLocation(FVector(0, 0, 70.0f));
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPArms"));
	SkeletalMeshComp->SetupAttachment(CameraComp);

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



