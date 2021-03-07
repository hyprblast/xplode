// Fill out your copyright notice in the Description page of Project Settings.


#include "EndBlockAnimNotify.h"
#include "GameFramework/Actor.h"
#include "xBaseCharacterInterface.h"

void UEndBlockAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Player = MeshComp->GetOwner();

	if (IsValid(MeshComp) && IsValid(Player) && Player->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()))
	{
		IxBaseCharacterInterface::Execute_SetPlayerIsBLocking(Player, false);
	}
}
