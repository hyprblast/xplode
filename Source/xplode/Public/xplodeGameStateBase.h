// Fill out your copyright notice in the Description page of Project Settings.
// Acts as the "model" in an MVC pattern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "xGameCamera.h"
#include "GameFramework/GameState.h"
#include "xplodeGameStateBase.generated.h"

/**
 *
 */
UCLASS()
class XPLODE_API AxplodeGameStateBase : public AGameState
{
	GENERATED_BODY()

public:
	AxplodeGameStateBase();

	UPROPERTY(Replicated)
	bool bShouldSpawnNewBall;

	UPROPERTY(Replicated)
		bool bBallSpawned;

};
