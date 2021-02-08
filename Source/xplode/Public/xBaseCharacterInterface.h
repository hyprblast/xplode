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
		int32 SetPlayerIsThrowing(bool bPlayerIsThrowing);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputAxisYawValue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 DetachBall(AxBallBase* Ball);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 AttachBall();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ThrowBall();
	
};
