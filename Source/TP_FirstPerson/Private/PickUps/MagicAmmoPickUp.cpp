// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicAmmoPickUp.h"

// Sets default values
AMagicAmmoPickUp::AMagicAmmoPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMagicAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("AmmoPickUp InitialLocation = %s"), *InitialLocation.ToString());
	TargetLocation = InitialLocation + FVector(0.0f, 0.0f, MaxZDiff);
}

// Called every frame
void AMagicAmmoPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewActorLocation;
	FVector CurrentLocation = GetActorLocation();
	switch (FloatingType)
	{
	case EPickUpFloatingType::Constant:
		NewActorLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, VerticalFloatingSpeed);
		break;
	case EPickUpFloatingType::Default:
		NewActorLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, VerticalFloatingSpeed);
		break;
	default:
		return;
	}

	SetActorLocation(NewActorLocation);
	
	UE_LOG(LogTemp, Display, TEXT("CurrentLocation = %s"), *CurrentLocation.ToString());
	FVector PositionsDiff = CurrentLocation - InitialLocation;
	
	double AbsPositionsDiff = FMath::Abs(PositionsDiff.Z);
	if (AbsPositionsDiff > MaxZDiff || FMath::IsNearlyEqual(CurrentLocation.Z, TargetLocation.Z))
	{
		bIsMovingUp = bIsMovingUp ? false : true;
		TargetLocation = InitialLocation;
		TargetLocation += bIsMovingUp ? FVector(0.0f, 0.0f, MaxZDiff) : FVector(0.0f, 0.0f, -MaxZDiff);
	}
}

