// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MagicCheatManagerExtension.generated.h"

UCLASS()
class TP_FIRSTPERSON_API UMagicCheatManagerExtension : public UCheatManagerExtension
{
	GENERATED_BODY()

	UFUNCTION(exec)
	void CheatSetDamageScale(int32 InDamageScale);
	
	int32 DamageScale = 1;

public:
	int32 GetDamageScale() const;
};
