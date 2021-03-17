// Fill out your copyright notice in the Description page of Project Settings.


#include "xPlayerController.h"
#include "GameFramework/Controller.h"
#include <GameFramework/Pawn.h>
#include <GameFramework/PawnMovementComponent.h>




AxPlayerController::AxPlayerController()
{
	static ConstructorHelpers::FClassFinder<UW_SelectTeamMaster> TeamSelectionnUIBPClass(TEXT("/Game/_Main/UI/W_SelectTeam.W_SelectTeam_C"));

	if (TeamSelectionnUIBPClass.Class != nullptr && TeamSelectionnUIBPClass.Succeeded())
	{
		SelectTeamWidgetUIClass = TeamSelectionnUIBPClass.Class;
	}
}

int32 AxPlayerController::ShowSelectTeam_Implementation()
{
	
	AxPlayerController::ClientShowTeamSelection();
	
	/*GetWorldTimerManager().SetTimer(
		ShowTeamSelectionWidgetTimerHandle, this, &AxPlayerController::ClientShowTeamSelection, 1.0f, false);*/
	
	return 1;
}

int32 AxPlayerController::SpawnPlayer_Implementation(TSubclassOf<AxBaseCharacter> PlayerToSpawnType, FTransform PlayerToSpawnTransform, FName PlayerTypeName)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	//spawnParams.Instigator = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AxBaseCharacter* SpawnedPlayer = GetWorld()->SpawnActor<AxBaseCharacter>(PlayerToSpawnType, PlayerToSpawnTransform, spawnParams);
	
	// Needs an event call here or to override to control the posses lifecycle
	Possess(SpawnedPlayer);

	// hide mouse on client
	ClientHideMouse();

	// Override transform on clients
	ClientSetSpawnTransform(PlayerToSpawnTransform);
	
	SpawnedPlayer->PlayerTypeName = PlayerTypeName;
	
	SpawnedPlayer->ClientSetPlayerTypeName(PlayerTypeName);

	//// Set top down default view
	SpawnedPlayer->MulticastSetTopDownViewSettings();
	
	return 1;
}

int32 AxPlayerController::SelectPlayerType_Implementation(FName TypeName)
{
	SelectTeamWidget->RemoveFromParent();
	
	ServerSelectPlayerType(TypeName);

	return 1;
}

void AxPlayerController::ServerSelectPlayerType_Implementation(FName TypeName)
{
	
	/*UE_LOG(LogTemp, Log, TEXT("Player type selected: %s"), *TypeName.ToString());*/

	/*The reason why we send this back to the gamemode and back to the controller is that we
	  want to make sure the player only gets spawned of the server to prevent cheating*/

	if (GameMode != nullptr && GameMode)
	{
		GameMode->RequestSpawnPlayerType(TypeName, this);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, GameState->GetMatchState().ToString());
		GameState->bShouldSpawnNewBall = !GameState->bBallSpawned;
	}
}

bool AxPlayerController::ServerSelectPlayerType_Validate(FName TypeName)
{
	return true;
}

//bool AxPlayerController::ClientShowTeamSelection_Validate()
//{
//	UE_LOG(LogTemp, Log, TEXT("VAlidate called"));
//	return true;
//}

void AxPlayerController::ClientShowTeamSelection_Implementation()
{
	
	/*FString value;
	UEnum::GetValueAsString(GetLocalRole(), value);*/

	//UE_LOG(LogTemp, Log, TEXT("Health Changed %s"), *FString::SanitizeFloat(Health));
	
	/*if (IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ClientShowTeamSelection_Implementation fire LocalPlayerController"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ClientShowTeamSelection_Implementation fire Remote Player Controller"));
	}*/

	if (SelectTeamWidgetUIClass && SelectTeamWidgetUIClass != nullptr)
	{
		SelectTeamWidget = CreateWidget<UW_SelectTeamMaster>(GetWorld(), SelectTeamWidgetUIClass);
		SelectTeamWidget->AddToViewport();
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
		/*UWidgetBlueprintLibrary::SetInputMode_GameAndUI(this, SelectTeamWidget);*/
	}

	/*GetWorldTimerManager().ClearTimer(ShowTeamSelectionWidgetTimerHandle);*/
	
}

void AxPlayerController::ClientSetSpawnTransform_Implementation(FTransform Transform)
{
	SetControlRotation(Transform.GetRotation().Rotator());
}

void AxPlayerController::ClientHideMouse_Implementation()
{
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

void AxPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AxplodeGameModeBase>(GetWorld()->GetAuthGameMode());
	
	GameState = Cast<AxplodeGameStateBase>(GetWorld()->GetGameState());
}

//void AxPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(AxPlayerController, SelectTeamWidgetUIClass);
//}
