// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MagicWidget.h"
#include "MagicPauseMenu.generated.h"

class UButton;

UCLASS()
class UMagicPauseMenu : public UMagicWidget
{
	GENERATED_BODY()
	
	UMagicPauseMenu();
	~UMagicPauseMenu();

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

private:
	UFUNCTION()
	void OnGamePaused();

	UFUNCTION()
	void OnResumeBtnClicked();

	UFUNCTION()
	void OnExitBtnClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> mResumeGameBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> mExitGameBtn;
};
