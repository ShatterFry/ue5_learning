// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MagicHUD.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class TP_FIRSTPERSON_API UMagicHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnBulletsCountChanged(int32 InBullets);
	
	UFUNCTION(BlueprintCallable)
	void SetBulletsText(int32 InBullets, bool InAnimated);

	UFUNCTION(BlueprintCallable)
	void SetHealthText(FText InText);

	UFUNCTION()
	void HandleTimerAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> mBulletsText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlusSign;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> IncreaseAnim;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> mCrosshair;
	
private:
	FTimerHandle AnimTimerHandle;
	
	float CurrentBulletsCount = 0.0f;
	float TargetBulletsCount = 0.0f;
	
	float AnimTimerRate = 0.25f;
};
