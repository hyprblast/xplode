// Copyright Epic Games, Inc. All Rights Reserved.


#include "xplodeGameModeBase.h"
#include "xplode/Public/xPlayerController.h"
#include "xBaseCharacter.h"
#include "xPlayerControllerInterface.h"
#include "EngineUtils.h"
#include "xPlayerStartBase.h"



AxplodeGameModeBase::AxplodeGameModeBase()
{
	DefaultPawnClass = nullptr;/*AxBaseCharacter::StaticClass();*/
	PlayerControllerClass = AxPlayerController::StaticClass();
}

void AxplodeGameModeBase::RequestSpawnPlayerType(FName TypeName, APlayerController* PlayerController)
{

	if (TypeName == TEXT("Blue"))
	{
		int32 Length = BlueSpanwPoints.Num();
		if (Length > 0)
		{
			int32 Rand = FMath::RandRange(0, Length - 1);
			
			FTransform Transform = BlueSpanwPoints[Rand]->GetTransform();
			
			if (PlayerController->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
			{
				//UE_LOG(LogTemp, Log, TEXT("Post Login"));
				/*IxPlayerControllerInterface::Execute_SpawnPlayer(TSubclassOf<;*/
			}
			

		}
	}
	else
	{
		if (RedSpawnPoints.Num() > 0)
		{
		}
	}
}


void AxplodeGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
	{
		//UE_LOG(LogTemp, Log, TEXT("Post Login"));
		IxPlayerControllerInterface::Execute_ShowSelectTeam(NewPlayer);
	}

	
}

void AxplodeGameModeBase::BeginPlay()
{
	int counter = 0;
	// iterate over all of our actors
	for (TActorIterator<AxPlayerStartBase> PlayerStartIterator(GetWorld()); PlayerStartIterator; ++PlayerStartIterator)
	{
		AxPlayerStartBase* PlayerStart = *PlayerStartIterator;
		
		if (PlayerStart && PlayerStart != nullptr)
		{
			counter++;
			//UE_LOG(Logemp, Log, TEXT("Player Start is %s"), *PlayerStart->GetClass()->GetName());

			if (counter <= 6)
			{
				BlueSpanwPoints.Add(PlayerStart);
			}
			else 
			{
				RedSpawnPoints.Add(PlayerStart);
			}

		}
		
		// ensure actor is not null
		// ignore self if found
		// ensure we find actors of a specific interface only
		//if (Actor && Actor != this && Actor->GetClass()->ImplementsInterface(UInteractiveActor::StaticClass()))
		//{
		//	// display all available tags for an actor
		//	for (FName Tag : Actor->Tags)
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Tag.ToString());
		//	}
		//}
	}
}


