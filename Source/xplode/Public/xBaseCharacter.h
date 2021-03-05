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
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Perception/AIPerceptionComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* LeftHandCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* RightHandCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* RightFootCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bHasBall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsThrowing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsPunching;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsKicking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsLeftPunch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsRightPunch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsTakingHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsLeftHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsRightHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsDead;

	UPROPERTY()
		UAudioComponent* ThrowPowerAudioComponent;

	UPROPERTY()
		UAudioComponent* WarnAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* ThrowPowerIncreaseSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* DieSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* WarningSoundFx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		FName PlayerTypeName;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsThrowing();  // This is the prototype declared in the interface
	virtual bool GetPlayerIsThrowing_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName GetPlayerType();
	virtual FName GetPlayerType_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsBlocking();
	virtual bool GetPlayerIsBlocking_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();
	virtual bool GetPlayerHasBall_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsDead();
	virtual bool GetPlayerIsDead_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);
	virtual int32 SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsPunching(bool bPlayerIsPunching);
	virtual int32 SetPlayerIsPunching_Implementation(bool bPlayerIsPunching) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsKicking(bool bPlayerIsKicking);
	virtual int32 SetPlayerIsKicking_Implementation(bool bPlayerIsKicking) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsLeftHit(bool bPlayerIsLeftHit);
	virtual int32 SetPlayerIsLeftHit_Implementation(bool bPlayerIsLeftHit) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsRightHit(bool bPlayerIsThrowing);
	virtual int32 SetPlayerIsRightHit_Implementation(bool bPlayerIsRightHit) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PushPlayer(FVector Force, bool bXOverride, bool bZOverride);
	virtual int32 PushPlayer_Implementation(FVector Force, bool bXOverride, bool bZOverride) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsGettingHit(bool bPlayerIsGettingHit);
	virtual int32 SetPlayerIsGettingHit_Implementation(bool bPlayerIsGettingHit) override;

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

	UFUNCTION()
		void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(Server, Reliable, WithValidation)
		void ServerThrowBall(FVector CameraFowardVector, bool bJustDropBall);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPLayerIsThrowing(bool bPlayerIsThrowing);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPLayerIsPunching(bool bPlayerIsPunching);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPLayerIsKicking(bool bPlayerIsKicking);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPlayerIsTakingHit(bool bPlayerIsTakingHit);

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVThrowAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPlayTPVThrowBallAnim();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPlayTPVPunchLeftAnim();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPlayTPVPunchRightAnim();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVPickupAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPlayTPVKickAnim();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVKickAnimation();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVPunchLeftAnimation();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVPunchRightAnimation();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVGettingPunchedLeftAnimation();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVGettingPunchedRightAnimation();

	UFUNCTION()
		void IncreaseThrowPower();

	UFUNCTION()
		void PlayPunchLeftAnim();

	UFUNCTION()
		void PlayKickAnim();

	UFUNCTION()
		void PlayPunchRightAnim();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnNewBallOnFPVMesh();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetTopDownViewSettings();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastKilledByExplosion();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastKilled();

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

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PunchLeftMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* KickMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PunchRightMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* GettingPunchedLeftMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* GettingPunchedRightMontage;

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

	UFUNCTION()
		void Die();

};
