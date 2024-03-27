// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_FirstPersonPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MagicCheatManagerExtension.h"
#include "Cheats/MagicCheatManager.h"
#include "GameFramework/CheatManager.h"

void ATP_FirstPersonPlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("Controller BeginPlay"));
	
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}

	if(CheatManager)
	{
		UE_LOG(LogTemp, Display, TEXT("CheatManager is valid!"));
		UMagicCheatManagerExtension* CheatManagerExtension = NewObject<UMagicCheatManagerExtension>(CheatManager);
		CheatManager->AddCheatManagerExtension(CheatManagerExtension);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("CheatManager is NOT valid!"));
	}

	MagicCheatManager = NewObject<UMagicCheatManager>(this, UMagicCheatManager::StaticClass());
	CheatManager = MagicCheatManager;
}
