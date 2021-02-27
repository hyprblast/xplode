// Fill out your copyright notice in the Description page of Project Settings.


#include "xplodeGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "xGameCamera.h"
#include "Chaos/Array.h"
#include "Net/UnrealNetwork.h"


AxplodeGameStateBase::AxplodeGameStateBase()
{
	SetReplicates(true);
}


void AxplodeGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AxplodeGameStateBase, bShouldSpawnNewBall);

}



