// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/DamageType.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "xBallBase.generated.h"

UCLASS()
class XPLODE_API AxBallBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AxBallBase();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/*UFUNCTION()
		void Shoot(FVector Velocity, float AccelerationMag);*/

	UFUNCTION()
		void StartTimer();

	UFUNCTION()
	void AddOverlap();
	
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetOwnerNoSee();

	UFUNCTION(Client, Reliable)
		void ClientStopWarn();

	// Called every frame
	/*virtual void Tick(float DeltaTime) override;*/

	/*UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SphereComp;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComp;*/

	UPROPERTY()
		AActor* LastPlayerOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", Replicated)
		bool bIsExplodeMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", Replicated)
		bool bIsExploding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", Replicated)
		float ExplodeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* ExplosionSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* WarningSoundFx;

	UPROPERTY()
		float Damage = 28.0f;

	UPROPERTY()
		UStaticMesh* SphereStaticMeshObject;

	UPROPERTY()
		UAudioComponent* AudioComponent;

	/*UFUNCTION()
	void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastExplode();
	
	UFUNCTION(Client, Reliable)
		void ClientWarn();


private:
	// Note to self: This timer needs to be set on server
	UPROPERTY()
		FTimerHandle ExplodeLevelIncrementTimerHandle;
	UPROPERTY()
		FTimerHandle RestoreCollisionTimerHandle;

	UPROPERTY()
		float ExplodesAt = 5;

	UFUNCTION()
		void ClearTimer();

	UFUNCTION()
		void SelfDestroy();

	UFUNCTION()
		void OnTimerElapsed();

	UFUNCTION()
		void RestoreCollision();

	UPROPERTY()
		bool bIsChangingCollision;

	UFUNCTION()
	void LoadVFXDynamicRefs();
	
	UFUNCTION()
		void SetStaticMesh();


};
