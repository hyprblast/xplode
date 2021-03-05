// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchDamageType.h"

UPunchDamageType::UPunchDamageType()
{
	bCausedByWorld = false;
	DamageImpulse = 500.f;
	Damage = 5.f;
	DamageTypeName = TEXT("Punch");
}

