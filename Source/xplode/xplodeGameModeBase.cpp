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
	if (!ReadyToStartMatch())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cannot start game, not enough players !"));
	}
	
	int32 Length = TypeName == TEXT("Blue") ? BlueSpanwPoints.Num() : RedSpawnPoints.Num();

	/*PlayerController->Tags.Add(TypeName);*/
	PlayerControllerList.Add(PlayerController);
	

	if (Length > 0 && PlayerUIClasses.Num() == 4)
	{
		int32 Rand = FMath::RandRange(0, Length - 1);
		int32 PlayerUIClassIndex = TypeName == TEXT("Blue") ? 0 : 1;
	

		FTransform Transform = TypeName == TEXT("Blue") ? BlueSpanwPoints[Rand]->GetTransform() : RedSpawnPoints[Rand]->GetTransform();
		/*FRotator Rotator = FRotator(0, TypeName == TEXT("Blue") ? -180.f : 180.f, 0);
		Transform.SetRotation(Rotator.Quaternion());*/

		if (PlayerController->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
		{
			IxPlayerControllerInterface::Execute_SpawnPlayer(PlayerController, PlayerUIClasses[PlayerUIClassIndex], Transform, TypeName);

			// Temporary: Spawn 2 bot from the opposite team
			int32 BotUIClassIndex = TypeName == TEXT("Blue") ? 3 : 2;
			Transform = TypeName == TEXT("Blue") ?  RedSpawnPoints[0]->GetTransform() : BlueSpanwPoints[0]->GetTransform();


			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AxBaseCharacter* SpawnedPlayer = GetWorld()->SpawnActor<AxBaseCharacter>(PlayerUIClasses[BotUIClassIndex], Transform, spawnParams);

			SpawnedPlayer->PlayerTypeName = TypeName == TEXT("Blue") ? TEXT("Red") : TEXT("Blue");

			Transform = TypeName == TEXT("Blue") ? RedSpawnPoints[1]->GetTransform() : BlueSpanwPoints[1]->GetTransform();

			AxBaseCharacter* SpawnedPlayer2 = GetWorld()->SpawnActor<AxBaseCharacter>(PlayerUIClasses[BotUIClassIndex], Transform, spawnParams);

			SpawnedPlayer2->PlayerTypeName = TypeName == TEXT("Blue") ? TEXT("Red") : TEXT("Blue");

			
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

	if (NewPlayer->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
	{
		//UE_LOG(LogTemp, Log, TEXT("Post Login"));
		IxPlayerControllerInterface::Execute_ShowSelectTeam(NewPlayer);
	}


}

void AxplodeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

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



bool AxplodeGameModeBase::ReadyToStartMatch_Implementation()
{
	//return Super::ReadyToStartMatch_Implementation();
	return NumPlayers >= MinPlayersNeededToStart;
}


void AxplodeGameModeBase::GetBluePrintPlayerClassRefs()
{
	static ConstructorHelpers::FClassFinder<AxBaseCharacter> BluePlayerUIBPClass(TEXT("/Game/_Main/Characters/Blueprints/BP_xBluePlayer.BP_xBluePlayer_C"));
	static ConstructorHelpers::FClassFinder<AxBaseCharacter> RedPlayerUIBPClass(TEXT("/Game/_Main/Characters/Blueprints/BP_xRedPlayer.BP_xRedPlayer_C"));
	static ConstructorHelpers::FClassFinder<AxBaseCharacter> BlueBotUIBPClass(TEXT("/Game/_Main/Characters/Blueprints/BP_xBlueBot.BP_xBlueBot_C"));
	static ConstructorHelpers::FClassFinder<AxBaseCharacter> RedBotUIBPClass(TEXT("/Game/_Main/Characters/Blueprints/BP_xRedBot.BP_xRedBot_C"));
	
	if (BluePlayerUIBPClass.Class != nullptr && BluePlayerUIBPClass.Succeeded() && RedPlayerUIBPClass.Class != nullptr && RedPlayerUIBPClass.Succeeded())
	{
		PlayerUIClasses.Add(BluePlayerUIBPClass.Class);
		PlayerUIClasses.Add(RedPlayerUIBPClass.Class);
		PlayerUIClasses.Add(BlueBotUIBPClass.Class);
		PlayerUIClasses.Add(RedBotUIBPClass.Class);
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

