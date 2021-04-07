// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/RectLightComponent.h"
#include "xWeaponBase.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "xWeaponPickupBase.generated.h"

UCLASS()
class XPLODE_API AxWeaponPickupBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AxWeaponPickupBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		UBoxComponent* BoxCollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		USkeletalMeshComponent* CrateComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		URectLightComponent* LightComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		UStaticMeshComponent* WeaponPickupComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		TSubclassOf<AxWeaponBase> WeaponBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defailt")
		bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* OpenCrateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystem* LootAuraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystem* PickupParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* PickupSoundFx;

	UPROPERTY(Replicated)
		bool bIsPickedUp;

	UPROPERTY(Replicated)
		bool bTriggerOpen;

	UFUNCTION()
	void OnWeaponPickedUp();

	UFUNCTION()
		void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UFUNCTION()
	void Open();
	
	UFUNCTION()
	void OpenMontageOnAnimationEnd(UAnimMontage* animMontage, bool bInterrupted);

	UPROPERTY()
	UParticleSystemComponent* SpawnedPickupParticle;

	UPROPERTY()
		UParticleSystemComponent* SpawnedAuraParticle;

};
