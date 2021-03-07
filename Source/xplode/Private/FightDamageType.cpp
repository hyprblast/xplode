// Fill out your copyright notice in the Description page of Project Settings.


#include "FightDamageType.h"

UFightDamageType::UFightDamageType()
{
	bCausedByWorld = false;
	DamageImpulse = 500.f;
	Damage = 5.f;
	DamageTypeName = TEXT("Fight");
}
