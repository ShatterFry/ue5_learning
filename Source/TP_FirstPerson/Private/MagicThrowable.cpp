// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicThrowable.h"

#include "Components/SphereComponent.h"
#include "DamageTypes/ThrowableDamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMagicThrowable::AMagicThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMagicThrowable::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
}

float AMagicThrowable::GetDamageRadius() const
{
	return DamageRadius;
}

float AMagicThrowable::GetImpulseStrength() const
{
	return ImpulseStrength;
}

// Called when the game starts or when spawned
void AMagicThrowable::BeginPlay()
{
	Super::BeginPlay();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	{
		FTimerHandle timerHandle;
        const FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AMagicThrowable::OnTriggered);
        TimerManager.SetTimer(timerHandle, delegate, 10.0f, false);	
	}
	
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMagicThrowable::DrawCurrentPosition);
	TimerManager.SetTimer(DebugTimerHandle, Delegate, 0.05f, true);
}

void AMagicThrowable::OnTriggered()
{
	UE_LOG(LogTemp, Display, TEXT("AMagicThrowable OnTriggered"));
	
	FVector DamageOrigin = GetActorLocation();
	DrawDebugSphere(GetWorld(), DamageOrigin, DamageRadius, 5, FColor::Emerald, false,
		60.0f);
	UE_VLOG_LOCATION(this, TEXT("MagicThrowable"), Log, DamageOrigin, DamageRadius, FColor::Blue, TEXT("Damage zone"));
	UClass* DamageTypeClass = UThrowableDamageType::StaticClass();
	//UClass* DamageTypeClass = UDamageType::StaticClass();
	
	const bool bDamageApplied = UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f,
		GetActorLocation(),
		DamageRadius,
		DamageTypeClass,
		{}, this, nullptr, false,
		ECC_Visibility);
	
	UE_LOG(LogTemp, Display, TEXT("bDamageApplied: %s"), bDamageApplied ? TEXT("YES") : TEXT("NO"));
	const FVector ActorLocation = GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(this, TriggerSound, GetActorLocation());
	APawn* NoiseInstigator = GetInstigator();
	check(NoiseInstigator);
	float MaxNoiseRange = 10000.0f;
	MakeNoise(1.0f, NoiseInstigator, ActorLocation, MaxNoiseRange);
	UGameplayStatics::SpawnEmitterAtLocation(this, TriggerParticles, GetActorLocation());

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(DebugTimerHandle);
	
	Destroy();
}

void AMagicThrowable::DrawCurrentPosition()
{
	UE_LOG(LogTemp, Display, TEXT("Draw debug figure"));
	DrawDebugSphere(GetWorld(), GetActorLocation(), 25.0f, 5, FColor::Purple, false, 10.0f);
	
#if ENABLE_VISUAL_LOG
	const ELogVerbosity::Type DefaultVerbosity = ELogVerbosity::Log;
	const FName CategoryName = TEXT("Throwable location");
	const FLinearColor Color = FLinearColor::Blue;
	const FString Text = FString::Printf(TEXT("%s"), *GetActorLocation().ToString());
	
	FVisualLogger::GeometryShapeLogf(this, CategoryName, DefaultVerbosity,
		GetActorLocation(), 50.0f, Color.ToFColor(true), TEXT("%s"), *Text);
#endif
}

void AMagicThrowable::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("AMagicThrowable::OnHit()"));
}

#if ENABLE_VISUAL_LOG
void AMagicThrowable::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	UE_LOG(LogTemp, Display, TEXT("void AMagicThrowable::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const"));
	
	//IVisualLoggerDebugSnapshotInterface::GrabDebugSnapshot(Snapshot);
	
	//const int32 CatIndex = Snapshot->Status.AddZeroed();
	//FVisualLogStatusCategory& PlaceableCategory = Snapshot->Status[CatIndex];

	FVisualLogStatusCategory PlaceableCategory;
	PlaceableCategory.Category = TEXT("GDC Sample");
	PlaceableCategory.Add(TEXT("Projectile Class"), GetClass()->GetName());

	Snapshot->Status.Add(PlaceableCategory);
}

void AMagicThrowable::DescribeSelfToVisLog(FVisualLogEntry* Snapshot) const
{
	UE_LOG(LogTemp, Display, TEXT("void AMagicThrowable::DescribeSelfToVisLog(FVisualLogEntry* Snapshot) const"));
}
#endif
