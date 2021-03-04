// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "xBaseDamageTypeInterface.h"
#include "xDamageTypeBase.generated.h"


UCLASS()
class XPLODE_API UxDamageTypeBase : public UDamageType, public IxBaseDamageTypeInterface
{
	GENERATED_BODY()

public:
	UPROPERTY()
		float Damage;

	UPROPERTY()
		FName DamageTypeName;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName GetDamageTypeName() const;
	FName GetDamageTypeName_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetDamageToApply() const;
	float GetDamageToApply_Implementation() const override;

};
