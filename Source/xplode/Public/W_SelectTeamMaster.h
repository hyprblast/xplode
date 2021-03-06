// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "xPlayerControllerInterface.h"
#include "W_SelectTeamMaster.generated.h"

class UButton;
UCLASS()
class XPLODE_API UW_SelectTeamMaster : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* RedButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* BlueButton;
	
	UFUNCTION()
		void OnRedClick();

	UFUNCTION()
		void OnBlueClick();

	virtual void NativeConstruct() override;

private:
	void BindButtonEvents();
	void SelectPlayerType(FName TypeName);
	
};
