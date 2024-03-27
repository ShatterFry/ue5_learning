// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicAmmoPickUp.generated.h"


UENUM(BlueprintType)
enum class EPickUpFloatingType : uint8
{
	None,
	Default,
	Constant
};


UCLASS()
class AMagicAmmoPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicAmmoPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector InitialLocation;
	FVector TargetLocation;

	bool bIsMovingUp = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "FloatingType != EPickUpFloatingType::None"))
	float MaxZDiff = 50.0f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "FloatingType != EPickUpFloatingType::None"))
	float VerticalFloatingSpeed = 25.0f;

	UPROPERTY(EditAnywhere)
	EPickUpFloatingType FloatingType = EPickUpFloatingType::Constant;
};
