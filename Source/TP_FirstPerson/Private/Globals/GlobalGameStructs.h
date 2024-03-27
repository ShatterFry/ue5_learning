// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DataTable.h"
#include "GlobalGameStructs.generated.h"

USTRUCT(BlueprintType)
struct FMagicAmmoGameplayDataTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 BulletsCapacity;

	UPROPERTY(EditAnywhere)
	int32 GrenadesCapacity;
};