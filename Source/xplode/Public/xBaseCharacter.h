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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bHasBall;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bIsThrowing;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsThrowing();  // This is the prototype declared in the interface
	virtual bool GetPlayerIsThrowing_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();  // This is the prototype declared in the interface
	virtual bool GetPlayerHasBall_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputAxisYawValue();  
	virtual float GetInputAxisYawValue_Implementation() override; 

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 DetachBall(AxBallBase* Ball);
	virtual int32 DetachBall_Implementation(AxBallBase* Ball) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall(AxBallBase* Ball);
	virtual int32 AttachBall_Implementation(AxBallBase* Ball) override;

	//Called from client, executed on server, withvalidation is required for this
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerThrowBall(FVector CameraLocation, FVector CameraFowardVector);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnNewBallOnFPVMesh();
	
	UFUNCTION()
		void DestroyBalls();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void MoveFoward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
		void Turn(float Value);
	UFUNCTION()
		void ThrowBall();
	UPROPERTY(BlueprintReadOnly, Category = "Default")
		float InputAxisYawValue = 0;
	UPROPERTY(Replicated)
		AxBallBase* FPVBall;
	UPROPERTY(Replicated)
		AxBallBase* TPVBall;

private:
	UFUNCTION()
	void AttachBallToTPVMesh(AxBallBase* Ball);

};
