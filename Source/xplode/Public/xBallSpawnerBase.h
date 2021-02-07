// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xBallBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
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

private:
	UFUNCTION()
	void SpawnBall();

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
