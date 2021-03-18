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
#include "Particles/ParticleSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Misc/Guid.h"
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

	//UPROPERTY(EditDefaultsOnly, Category = "Components")
	//	UPhysicsHandleComponent* PhysicsHandle;

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
		bool bIsFighting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsTakingHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		FGuid PlayerHittingMe;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bShouldSlide;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bools", Replicated)
		bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core", Replicated)
		FGuid PlayerId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	UParticleSystem* BloodParticles;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
		UParticleSystem* FightTrailParticle;*/

	UPROPERTY()
		UAudioComponent* ThrowPowerAudioComponent;

	UPROPERTY()
		UAudioComponent* BlockAudioComponent;

	UPROPERTY()
		UAudioComponent* WarnAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* ThrowPowerIncreaseSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* DieSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* WarningSoundFx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
		USoundCue* BlockSoundFx;

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
		bool GetPlayerIsFighting();
	virtual bool GetPlayerIsFighting_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsAutoFighting();
	virtual bool GetPlayerIsAutoFighting_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsGettingHit();
	virtual bool GetPlayerIsGettingHit_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();
	virtual bool GetPlayerHasBall_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsDead();
	virtual bool GetPlayerIsDead_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FGuid GetPlayerHittingMe();
	virtual FGuid GetPlayerHittingMe_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FGuid GetPlayerId();
	virtual FGuid GetPlayerId_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);
	virtual int32 SetPlayerIsThrowing_Implementation(bool bPlayerIsThrowing) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerAutofight(bool bPlayerAutofight);
	virtual int32 SetPlayerAutofight_Implementation(bool bPlayerAutofight) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsBLocking(bool bPlayerIsBlocking);
	virtual int32 SetPlayerIsBLocking_Implementation(bool bPlayerIsBlocking) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		uint8 AIBlock();
	virtual uint8 AIBlock_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsFighting(bool bPlayerIsFighting);
	virtual int32 SetPlayerIsFighting_Implementation(bool bPlayerIsFighting) override;

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
		int32 AttachBall();
	virtual int32 AttachBall_Implementation() override;

	UFUNCTION()
		void CallOnOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(Server, Reliable, WithValidation)
		void ServerThrowBall(FVector CameraFowardVector, bool bJustDropBall);

	UFUNCTION(Client, Reliable)
		void ClientThrowBall(FVector CameraFowardVector, bool bJustDropBall);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetPLayerIsThrowing(bool bPlayerIsThrowing);

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVThrowAnimation();

	UFUNCTION(Client, Reliable)
		void ClientPlayBlockSound();

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastBloodCloud(FVector ImpactPoint, FRotator Rotation);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPlayTPVThrowBallAnim();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFight(bool bIsKick, uint8 FightMoveIndex);

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayTPVPickupAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBlock();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastBlock();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFight(bool bIsKick, uint8 FightMoveIndex);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastGettingPunchedLogic(uint8 GettingPunchedMoveIndex);
	
	UFUNCTION()
		void IncreaseThrowPower();

	UFUNCTION()
		void Fight();

	UFUNCTION()
		void Kick();

	UFUNCTION()
		void Block();

	UFUNCTION()
		void BlockLogic();

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

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetActorRotation(FRotator TargetRotation);

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

	UPROPERTY(Replicated)
		bool bIsAutoFight;

	UPROPERTY(Replicated)
		bool bIsAttackMode;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TArray<UAnimMontage*> FightAnimMontages;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TArray<UAnimMontage*> FightKickAnimMontages;

	UPROPERTY(EditAnywhere, Category = "Animation")
		TArray<UAnimMontage*> GettingPunchedMontages;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* ThrowBallMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* ThrowBallMontageTPV;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* PickupBallMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* BlockMontage;


private:
	
	UPROPERTY()
	uint8 CurrentFightMoveIndex;
	
	UPROPERTY()
		UAnimMontage* LastPlayedGettingPunchedMontage;

	UPROPERTY()
		AxGameCamera* TopDownCam;

	UPROPERTY()
		UxCameraView CameraVieww;

	UPROPERTY()
		FTimerHandle AIDisableAttackModeTimerHandle;

	UPROPERTY()
		FTimerHandle AISetPlayerIsHitFalseTimerHandle;

	UPROPERTY()
		float PushPlayerFactor;

	UPROPERTY()
	UBehaviorTree* BehaviourTree;

	UPROPERTY()
		USoundCue* BackgroundSoundFx;

	UFUNCTION()
		void FindTopDownCamera();

	UFUNCTION()
		void AttachBallToTPVMesh();

	UFUNCTION()
		AxBallBase* SpawnBall(FTransform SpawnLocation);

	UFUNCTION()
		void LoadDynamicRefs();

	UFUNCTION()
		void Die();

	UFUNCTION()
		void FightLogic(uint8 FightMoveIndex);

	UFUNCTION()
		void KickLogic(uint8 FightMoveIndex);

	UFUNCTION()
	void AITraceAndAttack();

	UFUNCTION()
	void ResetBoolFlags(bool ResetBallPossession, bool ResetAtackMode);

	UFUNCTION()
	void AIDisableAttackMode();

	UFUNCTION()
	bool AreFightingRelatedMontagesPlaying();

};
