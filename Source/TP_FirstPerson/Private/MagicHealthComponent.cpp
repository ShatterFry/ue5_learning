// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicHealthComponent.h"

#include "MagicCheatManagerExtension.h"
#include "MagicThrowable.h"
#include "DamageTypes/ThrowableDamageType.h"
#include "GameFramework/CheatManager.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarMagicInstantEnemyKill(
	TEXT("InstantEnemyKill"),
	false,
	TEXT("Instant kill player enemied")
);

// Sets default values for this component's properties
UMagicHealthComponent::UMagicHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UMagicHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("OnTakeRadialDamage HealthComponent %s"), *GetName());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3600.0f, FColor::Yellow,
		FString(TEXT("OnTakeRadialDamage HealthComponent")));
	AActor* OwnerActor = GetOwner();
	FString OwnerActorName = OwnerActor->GetName();
	UE_LOG(LogTemp, Display, TEXT("HealthComponent OwnerActor: %s"), *GetOwner()->GetName());
	UPrimitiveComponent* HitComponent = HitInfo.Component.Get();
	UE_LOG(LogTemp, Display, TEXT("HitComponent: %s, Owner: %s"), *HitComponent->GetName(), *OwnerActorName);
	
	//float Radius = HitInfo.Distance;

	if(Cast<UThrowableDamageType>(DamageType))
	{
		const AMagicThrowable* ThrowableActor = Cast<AMagicThrowable>(DamageCauser);
		const float Radius = ThrowableActor->GetDamageRadius();
		const float Strength = ThrowableActor->GetImpulseStrength();
		constexpr ERadialImpulseFalloff Falloff = RIF_Linear;
		constexpr bool bVelChange = true;
		HitComponent->AddRadialImpulse(Origin, Radius, Strength, Falloff, bVelChange);
	}
}

void UMagicHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("HealthComponent OnTakePointDamage"));
}


// Called when the game starts
void UMagicHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	//Health = InitialHealth;
}

void UMagicHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,  AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("OnTakeAnyDamage HealthComponent %s"), *GetName());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3600.0f, FColor::Yellow,
		FString(TEXT("OnTakeAnyDamage HealthComponent")));
	UE_LOG(LogTemp, Display, TEXT("OwnerActor: %s"), *GetOwner()->GetName());
	
	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
	
	//APlayerController* playerController = CastChecked<APlayerController>(InstigatedBy);
	//APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	int32 DamageScale = 1;
	
	const UCheatManager* cheatManager = playerController->CheatManager;
	UCheatManagerExtension* CheatManagerExtension = cheatManager->FindCheatManagerExtension(UMagicCheatManagerExtension::StaticClass());
	if(const UMagicCheatManagerExtension* MagicCheatManagerExtension = Cast<UMagicCheatManagerExtension>(CheatManagerExtension))
	{
		DamageScale = MagicCheatManagerExtension->GetDamageScale();
	}
	
	UE_LOG(LogTemp, Display, TEXT("DamageScale = %d"), DamageScale);
	Health -= FMath::RoundToInt32(Damage * DamageScale);

	const bool bInstantKill = CVarMagicInstantEnemyKill.GetValueOnGameThread();
	UE_LOG(LogTemp, Display, TEXT("Instant enemy kill: %d"), bInstantKill);
	
	if(Health <= 0 || bInstantKill)
	{
		//DamagedActor->Destroy();
	}
}

int32 UMagicHealthComponent::GetInitialHealth() const
{
	return InitialHealth;
}

void UMagicHealthComponent::SetInitialHealth(int32 InInitialHealth)
{
	InitialHealth = InInitialHealth;
}

int32 UMagicHealthComponent::GetHealth() const
{
	return Health;
}

void UMagicHealthComponent::SetHealth(int32 InHealth)
{
	Health = InHealth;
}


// Called every frame
void UMagicHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

