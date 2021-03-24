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
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "xplodeGameStateBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
//#include "SmoothSync.h"
#include "xBallBase.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayWarningEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExplodeEvent);

UCLASS()
class XPLODE_API AxBallBase : public AActor
{
	GENERATED_BODY()

public:

	AxBallBase();
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/*UFUNCTION()
		void Shoot(FVector Velocity, float AccelerationMag);*/

	UFUNCTION()
		void StartTimer();

	UFUNCTION()
		void AddOverlapAndPhysics();

	UFUNCTION()
		void RemoveOverlapAndPhysics();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastExplode();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetOwnerNoSee();

	UFUNCTION()
		void StopWarn();

	UFUNCTION()
		void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Shoot(FVector Force);

	UFUNCTION()
		void PlayWarn();

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SphereComp;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USmoothSync* SmoothSyncComp;*/

		//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		//	USmoothSync* SmoothSyncComp;

		/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
			USmoothSync* SmoothSyncComp;*/

			/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
				UProjectileMovementComponent* ProjectileMovementComp;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComp;

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

	UPROPERTY()
		float Damage = 28.0f;

	UPROPERTY()
		UStaticMesh* SphereStaticMeshObject;

	UPROPERTY()
		UAudioComponent* AudioComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPlayWarningEvent OnWarning;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FExplodeEvent OnExploding;

	UPROPERTY(ReplicatedUsing = OnRep_Location)
		FVector ServerLocation;

	UPROPERTY(ReplicatedUsing = OnRep_Rotation)
		FQuat ServerRotation;

	UPROPERTY(ReplicatedUsing = OnRep_Velocity)
		FVector ServerVelocity;
	UPROPERTY(ReplicatedUsing = OnRep_AngularVelocity)
		FVector ServerAngularVelocity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAddSelfAsCameraTarget();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayScoreSFX();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayBallHitPostSFX();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayBallHitWallFX();

	UPROPERTY(Replicated)
		bool bSmoothingOn = true;

private:
	UPROPERTY()
		FTimerHandle ExplodeLevelIncrementTimerHandle;

	UPROPERTY()
		AxplodeGameStateBase* GameState;

	UPROPERTY()
		bool bScored;

	UPROPERTY()
		float ExplodesAt = 5;

	UPROPERTY()
		USoundCue* ScoreSoundFx;

	UPROPERTY()
		bool bInterpLocation;
	UPROPERTY()
		bool bInterpRotation;
	UPROPERTY()
		bool bInterpVelocity;
	UPROPERTY()
		bool bInterpAngularVelocity;

	UPROPERTY()
		USoundCue* HitPostSoundFx;

	UPROPERTY()
		USoundCue* HitWallSoundFx;

	UPROPERTY()
		float InterpRate = .85f;

	UPROPERTY()
		AActor* WhoHasMe;

	UFUNCTION()
		void ClearTimer();

	UFUNCTION()
		void SelfDestroy();

	UFUNCTION()
		void OnTimerElapsed();

	UFUNCTION()
		void LoadDynamicRefs();

	UFUNCTION()
		void SetStaticMesh();

	UFUNCTION()
		void DestroyAndSpawnNewBall();

	UFUNCTION()
		void OnRep_Location();
	UFUNCTION()
		void OnRep_Rotation();
	UFUNCTION()
		void OnRep_Velocity();
	UFUNCTION()
		void OnRep_AngularVelocity();

};
