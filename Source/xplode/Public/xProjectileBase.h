// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "xProjectileBase.generated.h"

UCLASS()
class XPLODE_API AxProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxProjectileBase();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereCollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystemComponent* ProjectileParticleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystem* ImpactParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComp;

	UFUNCTION()
		void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void Shoot(FVector Velocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void PostNetReceiveLocationAndRotation() override;


private:
	UPROPERTY()
		bool bHasHit;
	
};
