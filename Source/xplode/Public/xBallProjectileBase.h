// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "xBallProjectileBase.generated.h"

UCLASS()
class XPLODE_API AxBallProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AxBallProjectileBase();

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void RemoveCollision();

	UFUNCTION()
		void Shoot(FVector Velocity);

	UFUNCTION()
		void AutoDestroyAfterSecs(float Seconds);

	UPROPERTY(Replicated)
		FVector ProjectileVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SphereStaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY()
		UStaticMesh* SphereStaticMeshObject;


	/*virtual void Tick(float DeltaTime) override;*/



	//UFUNCTION()
	//void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostNetReceiveLocationAndRotation() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastServerTransform(FTransform ServerTransform, FVector ServerVelocity);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAddSelfAsCameraTarget();

private:
	UPROPERTY()
		FTimerHandle AutoDestroyTimerHandle;

	UPROPERTY()
		UAudioComponent* AudioComponent;

	UFUNCTION()
		void SetStaticMesh();

	UFUNCTION()
		void AutoDestroy();

	/*UFUNCTION()
		void LoadDynamicRefs();*/


	/*UFUNCTION()
		void BlastAndDestroyAfter(FVector EmiterSpawnLocation, FRotator EmiterSpawnRotation, EPhysicalSurface SurfaceType, float time);*/

		//UFUNCTION(NetMulticast, Reliable)
		//	void MulticastSelfDestruct();

		//UFUNCTION()
		//	void SelfDestroy();

		/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
			UParticleSystem* ImpactParticle;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
			USoundCue* ImpactSoundFx;*/

			//private:
			//	UStaticMesh* SphereStaticMeshObject;



};
