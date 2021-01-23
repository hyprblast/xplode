// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "xPlayerControllerInterface.h"
#include "xPlayerController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class XPLODE_API AxPlayerController : public APlayerController, public IxPlayerControllerInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ShowSelectedTeam();  // This is the prototype declared in the interface
	virtual int32 ShowSelectedTeam_Implementation() override; // This is the declaration of the implementation
};
