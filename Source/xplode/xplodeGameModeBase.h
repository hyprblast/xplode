// Copyright Epic Games, Inc. All Rights Reserved.
// NOTE: Gamemode only exists on server acts as the "controller" in MVC pattern

#pragma once

#include "CoreMinimal.h"
#include "xPlayerStartBase.h"
#include "xBaseCharacter.h"
#include "GameFramework/GameMode.h"
#include "xplodeGameModeBase.generated.h"


UCLASS()
class XPLODE_API AxplodeGameModeBase : public AGameMode
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
	
	virtual void BeginPlay() override;

	//virtual bool ReadyToStartMatch_Implementation() override;

private:
	UFUNCTION()
	void GetBluePrintPlayerClassRefs();

	UPROPERTY()
		TArray<AxPlayerStartBase*> BlueSpanwPoints;
	
	UPROPERTY()
		TArray<AxPlayerStartBase*> RedSpawnPoints;
	
	UPROPERTY()
		TArray<APlayerController*> PlayerControllerList;

	UPROPERTY()
	int32 MinPlayersNeededToStart = 1;

};
