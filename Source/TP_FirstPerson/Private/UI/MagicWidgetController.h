// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "MagicWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AMagicWidgetController : public AController
{
	GENERATED_BODY()
	
	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface
};
