// Fill out your copyright notice in the Description page of Project Settings.
// NOTE: PlayerControllers exists on server and client

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "xPlayerControllerInterface.h"
#include "Blueprint/UserWidget.h"
#include "W_SelectTeamMaster.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "xPlayerController.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class XPLODE_API AxPlayerController : public APlayerController, public IxPlayerControllerInterface
{
	GENERATED_BODY()


public:

	AxPlayerController();

	// Dynamic reference to the blueprint class
	/*UPROPERTY(Replicated)*/
	TSubclassOf<class UUserWidget> SelectTeamWidgetUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UW_SelectTeamMaster* SelectTeamWidget;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 ShowSelectTeam();  // This is the prototype declared in the interface
	virtual int32 ShowSelectTeam_Implementation() override; // This is the declaration of the implementation

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		int32 SelectPlayerType(FName TypeName);
	virtual int32 SelectPlayerType_Implementation(FName TypeName) override;

	 //Called from client, executed on server, withvalidation is required for this
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSelectPlayerType(FName TypeName);

	// Called from server, executed on client
	UFUNCTION(Client, Reliable)
		void ClientShowTeamSelection();

protected:
	FTimerHandle ShowTeamSelectionWidgetTimerHandle;
};
