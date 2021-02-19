// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "UObject/Interface.h"
#include "xPlayerControllerInterface.generated.h"


UINTERFACE()
class XPLODE_API UxPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class XPLODE_API IxPlayerControllerInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ShowSelectTeam();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SpawnPlayer(TSubclassOf<AxBaseCharacter> PlayerToSpawn, FTransform PlayerToSpawnTransform, FName PlayerTypeName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SelectPlayerType(FName TypeName);
	
};
