// Fill out your copyright notice in the Description page of Project Settings.


#include "xPlayerController.h"



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

//void AxPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(AxPlayerController, SelectTeamWidgetUIClass);
//}
