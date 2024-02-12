// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MagicLocalPlayerSaveGame.generated.h"

UCLASS()
class UMagicLocalPlayerSaveGame : public ULocalPlayerSaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	FVector ActorLocation;

	UPROPERTY()
	FRotator ActorRotation;

	UPROPERTY()
	bool bSaveGameInProgress = false;
	
public:
	void SetActorLocation(const FVector& InActorLocation) { ActorLocation = InActorLocation; }
	void SetActorRotation(const FRotator& InActorRotation) { ActorRotation = InActorRotation; }
	void SetSaveGameInProgress(const bool InProgress){ bSaveGameInProgress = InProgress; }

	bool IsSaveGameInProgress() const { return bSaveGameInProgress; }
	FVector GetActorLocation() const { return ActorLocation; }
	FRotator GetActorRotation() const { return ActorRotation; }
};
