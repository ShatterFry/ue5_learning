// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicHealthComponent.h"

#include "MagicCheatManagerExtension.h"
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

	// ...
	Health = InitialHealth;
}

void UMagicHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("OnTakeRadialDamage HealthComponent"));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3600.0f, FColor::Yellow,
		FString(TEXT("OnTakeRadialDamage HealthComponent")));
}


// Called when the game starts
void UMagicHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMagicHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,  AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("OnTakeAnyDamage HealthComponent"));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3600.0f, FColor::Yellow,
		FString(TEXT("OnTakeAnyDamage HealthComponent")));

	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
	
	//APlayerController* playerController = CastChecked<APlayerController>(InstigatedBy);
	//APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	
	const UCheatManager* cheatManager = playerController->CheatManager;
	const UMagicCheatManagerExtension* CheatExtension = CastChecked<UMagicCheatManagerExtension>(cheatManager->FindCheatManagerExtension(UMagicCheatManagerExtension::StaticClass()));
	const int32 DamageScale = CheatExtension->GetDamageScale();
	UE_LOG(LogTemp, Display, TEXT("DamageScale = %d"), DamageScale);
	Health -= FMath::RoundToInt32(Damage * DamageScale);

	bool bInstantKill = CVarMagicInstantEnemyKill.GetValueOnGameThread();
	UE_LOG(LogTemp, Display, TEXT("Instant enemy kill: %d"), bInstantKill);
	if(Health <= 0 || bInstantKill)
	{
		DamagedActor->Destroy();
	}
}


// Called every frame
void UMagicHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

