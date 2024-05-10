// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MagicHUD2.h"

AMagicHUD2::AMagicHUD2()
{
	UE_LOG(LogTemp, Display, TEXT("AMagicHUD2 Construct"));
}

void AMagicHUD2::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("AMagicHUD2 BeginPlay"));

	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();
	UGameViewportClient* GameViewportClient = GameInstance ? GameInstance->GetGameViewportClient() : nullptr;
	if (GameViewportClient)
	{
		//SlateWindow->AssignParentWidget(GameViewportClient->GetGameViewportWidget());
		//GameViewportClient->GetCanvas
	}
}

void AMagicHUD2::MyTestFunc()
{
	UE_LOG(LogTemp, Display, TEXT("void AMagicHUD2::MyTestFunc()"));
}
