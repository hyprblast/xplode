// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolShootDamageType.h"

UPistolShootDamageType::UPistolShootDamageType()
{
	bCausedByWorld = false;
	DamageImpulse = 500.f;
	Damage = 20.f;
	DamageTypeName = TEXT("Pistol Shot");
}
