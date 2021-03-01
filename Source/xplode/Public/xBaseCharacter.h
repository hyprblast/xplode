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
#include "GameFramework/SpringArmComponent.h"
#include "xCameraViewEnum.h"
#include "xGameCamera.h"
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
		float MaxThrowPower = 30000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults", Replicated)
		float MinThrowPower = 10000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Defaults", Replicated)
		float Health = 100;

	UPROPERTY(Replicated)
		bool bIsAddingThrowPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bHasBall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
		bool bIsThrowing;

	UPROPERTY()
		UAudioComponent* ThrowPowerAudioComponent;

	UPROPERTY()
		UAudioComponent* WarnAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* ThrowPowerIncreaseSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* WarningSoundFx;

	UPROPERTY()
		FName PlayerTypeName;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsThrowing();  // This is the prototype declared in the interface
	virtual bool GetPlayerIsThrowing_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();
	virtual bool GetPlayerHasBall_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);
	virtual int32 SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ThrowBall();
	virtual int32 ThrowBall_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PickupBall(AxBallBase* Ball);
	virtual int32 PickupBall_Implementation(AxBallBase* Ball) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputAxisYawValue();
	virtual float GetInputAxisYawValue_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 DetachBall(AxBallBase* Ball);
	virtual int32 DetachBall_Implementation(AxBallBase* Ball) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall();
	virtual int32 AttachBall_Implementation() override;


	UFUNCTION(Server, Reliable, WithValidation)
		void ServerThrowBall(FVector CameraFowardVector);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPLayerIsThrowing(bool bPlayerIsThrowing);

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
		void MulticastSetTopDownViewSettings();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastKilledByExplosion();

	UFUNCTION(Client, Reliable)
		void ClientActivateTopDownViewCam();

	UFUNCTION(Client, Reliable)
		void ClientSetPlayerTypeName(FName TypeName);

	UFUNCTION()
		void SubscribeToBallWarnEvent();

	UFUNCTION()
		void UnSubscribeToBallWarnEvent();

	UFUNCTION()
		void SubscribeToBallExplodeEvent();

	UFUNCTION()
		void UnSubscribeToBallExplodeEvent();

	UFUNCTION(Client, Reliable)
		void ClientPlayBallWarnEvent();

	UFUNCTION(Client, Reliable)
		void ClientStopPlayBallWarn();

	UFUNCTION()
		void OnBallWarn();

	UFUNCTION()
		void OnBallExploding();

	UFUNCTION()
		void SetHasBallFalse();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the Pawn is restarted (normally when possessed by a controller)
	/*virtual void Restart() override;*/

	/*virtual void PossessedBy(AController* NewController) override;*/

	UFUNCTION()
		void MoveFoward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Turn(float Value);

	UFUNCTION()
		void PlayThrowBallAnim();

	UFUNCTION()
		void CamToggle();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerIncreaseThrowPower();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetFirstPersonViewSettings();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetTopDownViewSettings();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetFirstPersonViewSettings();



	UFUNCTION(Client, Reliable)
		void ClientActivateFirstPersonViewCam();

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

private:
	UPROPERTY()
		AxGameCamera* TopDownCam;

	UPROPERTY()
		UxCameraView CameraVieww;

	UPROPERTY()
		FTimerHandle HasBallTimerHandle;

	UFUNCTION()
		void FindTopDownCamera();

	UFUNCTION()
		void AttachBallToTPVMesh();

	UFUNCTION()
		AxBallBase* SpawnBall(FTransform SpawnLocation);

	UFUNCTION()
		void LoadDynamicRefs();

	UFUNCTION()
		void DestroyBalls();

};
