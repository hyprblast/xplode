// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "xBallBase.generated.h"

UCLASS()
class XPLODE_API AxBallBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AxBallBase();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SphereComp;

	UPROPERTY()
	AActor* LastPlayerOwner;
	/*UFUNCTION()
	void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UStaticMesh* SphereStaticMeshObject;

	
	

};
