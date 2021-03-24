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
		bool GetPlayerIsPickingUpBall();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		uint8 SetPlayerHasBall(bool PlayerHasBall);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsThrowing();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsBlocking();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsFighting();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsAutoFighting();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsDead();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsSliding();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsKO();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		uint8 AIBlock();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerAutofight(bool bPlayerAutofight);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsBLocking(bool bPlayerIsBlocking);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsFighting(bool bPlayerIsFighting);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SetPlayerIsGettingHit(bool bPlayerIsGettingHit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PushPlayer(FVector Force, bool bXOverride, bool bZOverride);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool GetPlayerIsGettingHit();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FGuid GetPlayerHittingMe();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FGuid GetPlayerId();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputAxisYawValue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ThrowBall();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 PickupBall(AxBallBase* Ball);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName GetPlayerType();
	
};
