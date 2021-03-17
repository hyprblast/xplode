// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xBallBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Sound/SoundCue.h"
#include "xplodeGameStateBase.h"
#include "xBallSpawnerBase.generated.h"

UCLASS()
class XPLODE_API AxBallSpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxBallSpawnerBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SpawnerComp;
	UPROPERTY()
		UStaticMesh* SpawnerStaticMeshObject;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayBallSound();

private:
	
	UPROPERTY()
		USoundCue* PlayBallSoundFx;

	UPROPERTY()
		AxplodeGameStateBase* GameState;

	UFUNCTION()
		void LoadDynamicRefs();
	
	UFUNCTION()
	void SpawnBall();

public:	
	virtual void Tick(float DeltaTime) override;

};
