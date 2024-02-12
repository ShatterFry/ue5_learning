// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicCheatManagerExtension.h"

void UMagicCheatManagerExtension::CheatSetDamageScale(int32 InDamageScale)
{
	UE_LOG(LogTemp, Display, TEXT("CheatSetDamageScale: %d"), InDamageScale);
	DamageScale = InDamageScale;
}

int32 UMagicCheatManagerExtension::GetDamageScale() const
{
	return DamageScale;
}
