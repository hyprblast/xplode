// Copyright Epic Games, Inc. All Rights Reserved.
// NOTE: Gamemode only exists on server acts as the "controller" in MVC pattern

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "xPlayerStartBase.h"
#include "xBaseCharacter.h"
#include "xplodeGameModeBase.generated.h"


UCLASS()
class XPLODE_API AxplodeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AxplodeGameModeBase();
	UFUNCTION()
	void RequestSpawnPlayerType(FName TypeName, APlayerController* PlayerController);
	
	// Dynamic reference to the player blueprint character to spawn
	UPROPERTY()
	TArray<TSubclassOf<AxBaseCharacter>> PlayerUIClasses;


protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<AxPlayerStartBase*> BlueSpanwPoints;
	UPROPERTY()
	TArray<AxPlayerStartBase*> RedSpawnPoints;
	UPROPERTY()
	TArray<APlayerController*> PlayerControllerList;
	UFUNCTION()
	void GetBluePrintPlayerClassRefs();
	/*UFUNCTION()
	void SpawnBall();*/

};
