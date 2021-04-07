// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/DamageType.h"
#include "Camera/CameraShake.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "xProjectileBase.h"
#include "xWeaponBase.generated.h"

UCLASS()
class XPLODE_API AxWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Arm();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		bool bIsPistol;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		FVector ShellEjectPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		int CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		int InflictingDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
		USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
		TSubclassOf<UMatineeCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
		TSubclassOf<AxProjectileBase> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystem* MuzzleParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* ArmSoundFx;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
