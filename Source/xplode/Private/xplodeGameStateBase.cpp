// Fill out your copyright notice in the Description page of Project Settings.


#include "xplodeGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "xGameCamera.h"
#include "Chaos/Array.h"



void AxplodeGameStateBase::BeginPlay()
{
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AxGameCamera::StaticClass(), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		GameCamera = Cast<AxGameCamera>(FoundCameras[0]);
	}
}
