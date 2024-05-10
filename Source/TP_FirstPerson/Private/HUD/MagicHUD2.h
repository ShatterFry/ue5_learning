// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MagicHUD2.generated.h"

/**
 * 
 */
UCLASS()
class AMagicHUD2 : public AHUD
{
	GENERATED_BODY()
	
public:
	AMagicHUD2();

protected:
	void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void MyTestFunc();
};
