// Fill out your copyright notice in the Description page of Project Settings.
// Acts as the "model" in an MVC pattern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include <GameFramework/Actor.h>
#include "xGameCamera.h"
#include "xplodeGameStateBase.generated.h"

/**
 *
 */
UCLASS()
class XPLODE_API AxplodeGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
		AxGameCamera* GameCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
