// Fill out your copyright notice in the Description page of Project Settings.


#include "xWeaponBase.h"

// Sets default values
AxWeaponBase::AxWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetupAttachment(RootComponent);

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponActualSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(DefaultSceneRoot);
	WeaponSkeletalMesh->CastShadow = false;

}

void AxWeaponBase::Fire()
{

}

// Called when the game starts or when spawned
void AxWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AxWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

