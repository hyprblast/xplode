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
	int32 SetPlayerHasBall(bool bPlayerHasBall);
};