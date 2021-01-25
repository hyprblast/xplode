// Fill out your copyright notice in the Description page of Project Settings.


#include "W_SelectTeamMaster.h"



void UW_SelectTeamMaster::OnRedClick()
{
	SelectPlayerType(TEXT("Red"));
}

void UW_SelectTeamMaster::OnBlueClick()
{
	SelectPlayerType(TEXT("Blue"));
}

void UW_SelectTeamMaster::NativeConstruct()
{
	Super::NativeConstruct();
	BindButtonEvents();
}

void UW_SelectTeamMaster::BindButtonEvents()
{
	if (!RedButton->OnClicked.IsBound())
	{
		RedButton->OnClicked.AddDynamic(this, &UW_SelectTeamMaster::OnRedClick);
	}

	if (!BlueButton->OnClicked.IsBound())
	{
		BlueButton->OnClicked.AddDynamic(this, &UW_SelectTeamMaster::OnBlueClick);
	}
}

void UW_SelectTeamMaster::SelectPlayerType(FName TypeName)
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (PlayerController &&
		PlayerController != nullptr &&
		PlayerController->GetClass()->ImplementsInterface(UxPlayerControllerInterface::StaticClass()))
	{
		IxPlayerControllerInterface::Execute_SelectPlayerType(PlayerController, TypeName);
	}
}
