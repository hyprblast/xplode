// Copyright Epic Games, Inc. All Rights Reserved.


#include "xplodeGameModeBase.h"
#include "xplode/Public/xPlayerController.h"
#include "xBaseCharacter.h"



AxplodeGameModeBase::AxplodeGameModeBase()
{
	DefaultPawnClass = AxBaseCharacter::StaticClass();
	PlayerControllerClass = AxPlayerController::StaticClass();
}