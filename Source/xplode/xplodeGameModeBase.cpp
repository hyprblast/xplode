// Copyright Epic Games, Inc. All Rights Reserved.


#include "xplodeGameModeBase.h"
#include "xplode/Public/xPlayerController.h"
#include "xBaseCharacter.h"
#include "xPlayerControllerInterface.h"



AxplodeGameModeBase::AxplodeGameModeBase()
{
	DefaultPawnClass = nullptr;/*AxBaseCharacter::StaticClass();*/
	PlayerControllerClass = AxPlayerController::StaticClass();
}

void AxplodeGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Post Login"));
		IxPlayerControllerInterface::Execute_ShowSelectTeam(NewPlayer);
	}

	
}



