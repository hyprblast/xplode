// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "xBaseCharacterInterface.h"
#include "xBaseCharacter.generated.h"

class UCameraComponent;

UCLASS()
class XPLODE_API AxBaseCharacter : public ACharacter, public IxBaseCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AxBaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bHasBall;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();  // This is the prototype declared in the interface
	virtual bool GetPlayerHasBall_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerHasBall(bool bPlayerHasBall);
	virtual int32 SetPlayerHasBall_Implementation(bool bPlayerHasBall) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall(AxBallBase* Ball);
	virtual int32 AttachBall_Implementation(AxBallBase* Ball) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveFoward(float Value);
	void MoveRight(float Value);

private:
	void AttachBallToTPVMesh(AxBallBase* Ball);
	void SpawnNewBallOnFPVMesh();
	
	UPROPERTY()
	AxBallBase* BallInHand;
	

};
