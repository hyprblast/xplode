// Fill out your copyright notice in the Description page of Project Settings.


#include "xDamageTypeBase.h"


FName UxDamageTypeBase::GetDamageTypeName_Implementation() const
{
	return DamageTypeName;
}

float UxDamageTypeBase::GetDamageToApply_Implementation() const
{
	return Damage;
}
