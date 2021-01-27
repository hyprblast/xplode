// Copyright Epic Games, Inc. All Rights Reserved.
// NOTE: Gamemode only exists on server

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "xPlayerStartBase.h"
#include "xplodeGameModeBase.generated.h"


UCLASS()
class XPLODE_API AxplodeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AxplodeGameModeBase();
	void RequestSpawnPlayerType(FName TypeName, APlayerController* PlayerController);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<AxPlayerStartBase*> BlueSpanwPoints;
	TArray<AxPlayerStartBase*> RedSpawnPoints;

};
