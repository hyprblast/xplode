// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "xGoalBase.generated.h"

UCLASS()
class XPLODE_API AxGoalBase : public AActor
{
	GENERATED_BODY()

public:
	AxGoalBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* BoxCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComp;

	/*UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	/*UFUNCTION()
		void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
