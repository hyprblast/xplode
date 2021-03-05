// Fill out your copyright notice in the Description page of Project Settings.


#include "KickDamageType.h"

UKickDamageType::UKickDamageType()
{
	bCausedByWorld = false;
	DamageImpulse = 600.f;
	Damage = 8.f;
	DamageTypeName = TEXT("Kick");
}
