// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "xBaseCharacterInterface.generated.h"

UINTERFACE()
class XPLODE_API UxBaseCharacterInterface : public UInterface
{
	GENERATED_BODY()

};

class XPLODE_API IxBaseCharacterInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerHasBall();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsThrowing();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsBlocking();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsDead();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsPunching(bool bPlayerIsPunching);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsKicking(bool bPlayerIsKicking);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsLeftHit(bool bPlayerIsLeftHit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsRightHit(bool bPlayerIsRightHit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsGettingHit(bool bPlayerIsGettingHit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PushPlayer(FVector Force, bool bXOverride, bool bZOverride);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputAxisYawValue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 DetachBall(AxBallBase* Ball);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ThrowBall();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PickupBall(AxBallBase* Ball);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName GetPlayerType();
	
};
