// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicHUD.h"

#include "Components/TextBlock.h"

void UMagicHUD::SetBulletsText(int32 InBullets)
{
	const FString formattedText = FString::Printf(TEXT("Bullets: %d"), InBullets);
	mBulletsText->SetText(FText::FromString(formattedText));
}
