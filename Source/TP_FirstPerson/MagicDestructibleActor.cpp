// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicDestructibleActor.h"

#include "MagicHealthComponent.h"

// Sets default values
AMagicDestructibleActor::AMagicDestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UMagicHealthComponent>(TEXT("ActorHealth"));
	OnTakeRadialDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakeRadialDamage);
	//OnTakePointDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnCharacterReceivedDamage);
	OnTakeAnyDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakeAnyDamage);
}

// Called when the game starts or when spawned
void AMagicDestructibleActor::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->RegisterComponent();
	HealthComponent->Activate();

	AActor* ownerActor = HealthComponent->GetOwner();
	if(ownerActor)
	{
		FString ownerName = HealthComponent->GetOwner()->GetName();
		UE_LOG(LogTemp, Display, TEXT("Owner: %s"), *ownerName);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Owner is not set!"));
	}
}

// Called every frame
void AMagicDestructibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

