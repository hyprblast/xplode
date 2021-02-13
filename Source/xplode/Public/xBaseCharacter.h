// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "xBaseCharacterInterface.h"
#include "GameFramework/DamageType.h"
#include "Animation/AnimMontage.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "xBaseCharacter.generated.h"

class UCameraComponent;

UCLASS()
class XPLODE_API AxBaseCharacter : public ACharacter, public IxBaseCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AxBaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults", Replicated)
		float ThrowPower = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults", Replicated)
		float MaxThrowPower = 2000;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults", Replicated)
		float Health = 100;

	UPROPERTY(Replicated)
		bool bIsAddingThrowPower;

	UPROPERTY()
		bool bIsCatchMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bHasBall;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bIsThrowing;

	UPROPERTY()
		UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* ThrowPowerIncreaseSoundFx;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsThrowing();  // This is the prototype declared in the interface
	virtual bool GetPlayerIsThrowing_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();  // This is the prototype declared in the interface
	virtual bool GetPlayerHasBall_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsInCatchMode();  // This is the prototype declared in the interface
	virtual bool GetPlayerIsInCatchMode_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);  // This is the prototype declared in the interface
	virtual int32 SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing) override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ThrowBall();  // This is the prototype declared in the interface
	virtual int32 ThrowBall_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PickupBall();  // This is the prototype declared in the interface
	virtual int32 PickupBall_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputAxisYawValue();  
	virtual float GetInputAxisYawValue_Implementation() override; 

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 DetachBall(AxBallBase* Ball);
	virtual int32 DetachBall_Implementation(AxBallBase* Ball) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall();
	virtual int32 AttachBall_Implementation() override;

	//Called from client, executed on server, withvalidation is required for this
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerThrowBall(FVector CameraLocation, FVector CameraFowardVector);
	//Called from client, executed on server, withvalidation is required for this
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPLayerIsThrowing(bool bPlayerIsThrowing);
	//Called from server, executed on each copies of the actor, including server
	UFUNCTION(NetMulticast, Reliable)
		void MulticastPlayTPVThrowAnimation();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPlayTPVThrowBallAnim();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastPlayTPVPickupAnimation();
	
	UFUNCTION()
	void IncreaseThrowPower();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnNewBallOnFPVMesh();
	
	UFUNCTION(NetMulticast, Reliable)
		void MulticastDestroyBalls();



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
		void PlayThrowBallAnim();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetCatchMode();
	UFUNCTION()
		void UnSetCatchMode();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerIncreaseThrowPower();
	UFUNCTION()
		void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(BlueprintReadOnly, Category = "Default")
		float InputAxisYawValue = 0;
	UPROPERTY(Replicated)
		AxBallBase* FPVBall;
	UPROPERTY(Replicated)
		AxBallBase* TPVBall;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* ThrowBallMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* ThrowBallMontageTPV;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PickupBallMontage;
	/*UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PickupBallMontageTPV;*/

private:
	UFUNCTION()
	void AttachBallToTPVMesh();
	UPROPERTY()
		FTimerHandle CatchModeTimerHandle;

	UFUNCTION()
	void TempChangeCollision(AxBallProjectileBase* BallProjectile);

	UFUNCTION()
	AxBallBase* SpawnBall(FTransform SpawnLocation);

	UFUNCTION()
	void LoadVFXDynamicRefs();

	UFUNCTION()
	void DestroyBalls();
};
