// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "xBaseDamageTypeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UxBaseDamageTypeInterface : public UInterface
{
	GENERATED_BODY()
};


class XPLODE_API IxBaseDamageTypeInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName GetDamageTypeName() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetDamageToApply() const;

};
