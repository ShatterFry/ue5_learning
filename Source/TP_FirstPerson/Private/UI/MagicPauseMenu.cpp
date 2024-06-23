// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MagicPauseMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "TP_FirstPersonCharacter.h"

UMagicPauseMenu::UMagicPauseMenu()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
}

UMagicPauseMenu::~UMagicPauseMenu()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
}

void UMagicPauseMenu::OnGamePaused()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));
}

void UMagicPauseMenu::NativeConstruct()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));

	FCoreDelegates::ApplicationWillDeactivateDelegate.AddUObject(this, &UMagicPauseMenu::OnGamePaused);

	if (IsValid(mResumeGameBtn))
	{
		mResumeGameBtn->OnClicked.AddUniqueDynamic(this, &UMagicPauseMenu::OnResumeBtnClicked);
	}

	if (IsValid(mExitGameBtn))
	{
		mExitGameBtn->OnClicked.AddUniqueDynamic(this, &UMagicPauseMenu::OnExitBtnClicked);
	}
}

void UMagicPauseMenu::NativeDestruct()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));

	FCoreDelegates::ApplicationWillDeactivateDelegate.RemoveAll(this);

	if (IsValid(mResumeGameBtn))
	{
		mResumeGameBtn->OnClicked.RemoveAll(this);
	}

	if (IsValid(mExitGameBtn))
	{
		mExitGameBtn->OnClicked.RemoveAll(this);
	}
}

void UMagicPauseMenu::OnResumeBtnClicked()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ATP_FirstPersonCharacter* PlayerCharacter = Cast<ATP_FirstPersonCharacter>(PlayerController->GetPawn());
	PlayerCharacter->OnGamePauseRequested();
}

void UMagicPauseMenu::OnExitBtnClicked()
{
	UE_LOG(LogTemp, Display, TEXT("%s"), ANSI_TO_TCHAR(__FUNCTION__));

	//constexpr bool bForceExit = false;
	//FPlatformMisc::RequestExit(bForceExit);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->ConsoleCommand(TEXT("Exit"), true);
	}
}
