// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MagicCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class UMagicCheatManager : public UCheatManager
{
	GENERATED_BODY()

	UFUNCTION(Exec)
	void CheatToggleFlyMode();

	UFUNCTION(Exec)
	void CheatAddBullets(int32 InBullets);

	UFUNCTION(Exec)
	void CheatSetCrosshairType(int32 InCrosshairType);
};
