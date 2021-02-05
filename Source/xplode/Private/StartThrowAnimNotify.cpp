// Fill out your copyright notice in the Description page of Project Settings.


#include "StartThrowAnimNotify.h"
#include "GameFramework/Actor.h"
#include "xBaseCharacterInterface.h"

void UStartThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Player = MeshComp->GetOwner();

	if (IsValid(MeshComp) && IsValid(Player) && Player->GetClass()->ImplementsInterface(UxBaseCharacterInterface::StaticClass()))
	{
		/*UE_LOG(LogTemp, Log, TEXT("Notify"));*/
		IxBaseCharacterInterface::Execute_ThrowBall(Player);
		
	}
}