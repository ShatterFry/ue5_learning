// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TP_FirstPersonPlayerController.generated.h"

class UInputMappingContext;
class UMagicCheatManager;
class AMagicWidgetController;

UCLASS()
class TP_FIRSTPERSON_API ATP_FirstPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UMagicCheatManager> MagicCheatManager;

	UPROPERTY()
	TObjectPtr<AMagicWidgetController> WidgetController;
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	// End Actor interface

private:
	UFUNCTION()
	void OnGamePauseRequested();
};
