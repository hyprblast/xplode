// Copyright Epic Games, Inc. All Rights Reserved.


#include "xplodeGameModeBase.h"
#include "xplode/Public/xPlayerController.h"
#include "xBaseCharacter.h"
#include "xPlayerControllerInterface.h"
#include "EngineUtils.h"
#include "xPlayerStartBase.h"
#include "xBallBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "xBaseCharacterInterface.h"
#include "xplodeGameStateBase.h"



AxplodeGameModeBase::AxplodeGameModeBase()
{
	DefaultPawnClass = nullptr;/*AxBaseCharacter::StaticClass();*/
	PlayerControllerClass = AxPlayerController::StaticClass();
	GameStateClass = AxplodeGameStateBase::StaticClass();

	GetBluePrintPlayerClassRefs();
}

void AxplodeGameModeBase::RequestSpawnPlayerType(FName TypeName, APlayerController* PlayerController)
{
	int32 Length = TypeName == TEXT("Blue") ? BlueSpanwPoints.Num() : RedSpawnPoints.Num();

	if (Length > 0 && PlayerUIClasses.Num() == 2)
	{
		int32 Rand = FMath::RandRange(0, Length - 1);
		int32 PlayerUIClassIndex = TypeName == TEXT("Blue") ? 0 : 1;

		FTransform Transform = TypeName == TEXT("Blue") ? BlueSpanwPoints[Rand]->GetTransform() : RedSpawnPoints[Rand]->GetTransform();

		if (PlayerController->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
		{
			//UE_LOG(LogTemp, Log, TEXT("Post Login"));
			IxPlayerControllerInterface::Execute_SpawnPlayer(PlayerController, PlayerUIClasses[PlayerUIClassIndex], Transform);
		}
	}
}


//void AxplodeGameModeBase::ResetPlayerWhoHasBall()
//{
//	for (int32 i = 0; i < PlayerControllerList.Num(); i++)
//	{
//		APlayerController* PlayerController = PlayerControllerList[i];
//		APawn* Pawn = PlayerController->GetPawn();
//
//		if (Pawn && 
//			Pawn != nullptr && 
//			Pawn->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()) &&
//			IxBaseCharacterInterface::Execute_GetPlayerHasBall(Pawn))
//		{
//			IxBaseCharacterInterface::Execute_SetPlayerHasBall(Pawn, false);
//		}
//
//	}
//}


void AxplodeGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//PlayerControllerList.Add(NewPlayer);

	if (NewPlayer->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
	{
		//UE_LOG(LogTemp, Log, TEXT("Post Login"));
		IxPlayerControllerInterface::Execute_ShowSelectTeam(NewPlayer);
	}


}

void AxplodeGameModeBase::BeginPlay()
{
	int counter = 0;

	/*SpawnBall();*/


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

void AxplodeGameModeBase::GetBluePrintPlayerClassRefs()
{
	static ConstructorHelpers::FClassFinder<AxBaseCharacter> BluePlayerUIBPClass(TEXT("/Game/_Main/Characters/Blueprints/BP_xBluePlayer.BP_xBluePlayer_C"));
	static ConstructorHelpers::FClassFinder<AxBaseCharacter> RedPlayerUIBPClass(TEXT("/Game/_Main/Characters/Blueprints/BP_xRedPlayer.BP_xRedPlayer_C"));

	if (BluePlayerUIBPClass.Class != nullptr && BluePlayerUIBPClass.Succeeded() && RedPlayerUIBPClass.Class != nullptr && RedPlayerUIBPClass.Succeeded())
	{
		PlayerUIClasses.Add(BluePlayerUIBPClass.Class);
		PlayerUIClasses.Add(RedPlayerUIBPClass.Class);
	}
}

// This cannot happen here as this object will only exist on the server
//void AxplodeGameModeBase::SpawnBall()
//{
//	// This logic is temporary and only for prototype
//
//	FActorSpawnParameters SpawnParams;
//	/*SpawnParams.Owner = this;*/
//
//	//TPV Ball
//	AxBallBase* Ball = GetWorld()->SpawnActor<AxBallBase>(AxBallBase::StaticClass(), FVector(200, 177, 227), FRotator(0, 0, 0), SpawnParams);
//	Ball->SphereComp->SetOwnerNoSee(true); 
//	
//
//}

