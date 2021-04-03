// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/DamageType.h"
#include "Camera/CameraShake.h"
#include "xWeaponBase.generated.h"

UCLASS()
class XPLODE_API AxWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		bool bIsAutomatic;
	
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

	UFUNCTION()
	void Fire();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
