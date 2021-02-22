// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "xGameCamera.generated.h"

/**
 *
 */
UCLASS()
class XPLODE_API AxGameCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AxGameCamera();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetTurnYaw(float TurnYawVal);

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SphereComp;

	UPROPERTY()
		UStaticMesh* SphereStaticMeshObject;*/
	UPROPERTY()
		AActor* FollowActor;



	

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FRotator CamInitialRotation;
	
	UPROPERTY()
		float TurnYaw;

		UPROPERTY()
		bool bShouldTurn;
};
