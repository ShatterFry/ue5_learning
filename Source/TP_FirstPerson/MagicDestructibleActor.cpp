// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicDestructibleActor.h"

#include "MagicHealthComponent.h"

void AMagicDestructibleActor::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("AMagicDestructibleActor::OnTakeAnyDamageCallback"));
}

// Sets default values
AMagicDestructibleActor::AMagicDestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnTakeAnyDamage.AddUniqueDynamic(this, &AMagicDestructibleActor::OnTakeAnyDamageCallback);

	HealthComponent = CreateDefaultSubobject<UMagicHealthComponent>(TEXT("ActorHealth"));
	OnTakeRadialDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakeRadialDamage);
	OnTakePointDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakePointDamage);
	OnTakeAnyDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakeAnyDamage);
}

#if ENABLE_VISUAL_LOG
void AMagicDestructibleActor::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	IVisualLoggerDebugSnapshotInterface::GrabDebugSnapshot(Snapshot);
	//Super::GrabDebugSnapshot(Snapshot);
	const int32 CatIndex = Snapshot->Status.AddZeroed();
	FVisualLogStatusCategory& PlaceableCategory = Snapshot->Status[CatIndex];
	PlaceableCategory.Category = TEXT("GDC Sample");
	PlaceableCategory.Add(TEXT("Destrictible Actor Class"), GetClass() != nullptr ? GetClass()->GetName() : TEXT("None"));
}
#endif

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

