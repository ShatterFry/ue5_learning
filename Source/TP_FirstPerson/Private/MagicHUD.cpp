// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicHUD.h"

#include "Components/TextBlock.h"

void UMagicHUD::OnBulletsCountChanged(int32 InBullets)
{
	if(AnimTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerHandle);
		StopAllAnimations();
		PlusSign->SetVisibility(ESlateVisibility::Hidden);
		SetBulletsText(TargetBulletsCount, false);
	}
	
	TargetBulletsCount = static_cast<float>(InBullets);
	SetBulletsText(InBullets, true);
}

void UMagicHUD::SetBulletsText(int32 InBullets, bool InAnimated)
{
	if(InAnimated)
	{
		PlusSign->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayAnimation(IncreaseAnim, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f, true);
		
		FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &UMagicHUD::HandleTimerAnimation);
		GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, delegate, AnimTimerRate, true);
		return;
	}

	CurrentBulletsCount = TargetBulletsCount;
	const FString formattedText = FString::Printf(TEXT("Bullets: %d"), InBullets);
	mBulletsText->SetText(FText::FromString(formattedText));
}

void UMagicHUD::SetHealthText(FText InText)
{
	HealthText->SetText(InText);
}

void UMagicHUD::HandleTimerAnimation()
{
	UE_LOG(LogTemp, Display, TEXT("HandleTimerAnimation"));

	
	if(FMath::IsNearlyEqual(CurrentBulletsCount, TargetBulletsCount))
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimTimerHandle);
		StopAllAnimations();
		PlusSign->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	//CurrentBulletsCount = FMath::FInterpTo(CurrentBulletsCount, TargetBulletsCount, AnimTimerRate, 0.25f);
	CurrentBulletsCount = FMath::FInterpConstantTo(CurrentBulletsCount, TargetBulletsCount, AnimTimerRate, 4.0f);
	
	const FString formattedText = FString::Printf(TEXT("Bullets: %d"), FMath::RoundToInt32(CurrentBulletsCount));
	mBulletsText->SetText(FText::FromString(formattedText));
}
