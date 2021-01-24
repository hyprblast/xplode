// Copyright Epic Games, Inc. All Rights Reserved.
// NOTE: Gamemode only exists on server

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "xplodeGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class XPLODE_API AxplodeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AxplodeGameModeBase();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

};
