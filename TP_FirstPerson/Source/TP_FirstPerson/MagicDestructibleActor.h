// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicDestructibleActor.generated.h"

class UMagicHealthComponent;

UCLASS()
class TP_FIRSTPERSON_API AMagicDestructibleActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Magic, meta=(AllowPrivateAccess = "true"))
	UMagicHealthComponent* HealthComponent;
	
public:	
	// Sets default values for this actor's properties
	AMagicDestructibleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
