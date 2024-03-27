// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
#include "MagicThrowable.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TP_FIRSTPERSON_API AMagicThrowable : public AActor, public IVisualLoggerDebugSnapshotInterface 
{
	GENERATED_BODY()
	
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AMagicThrowable();

	float GetDamageRadius() const;
	float GetImpulseStrength() const;
	
#if ENABLE_VISUAL_LOG
	//~ Begin IVisualLoggerDebugSnapshotInterface interface
	// Adds information about this Actor to the Visual Logger.
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
	virtual void DescribeSelfToVisLog(struct FVisualLogEntry* Snapshot) const;
	//~ End IVisualLoggerDebugSnapshotInterface interface
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USoundBase* TriggerSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TriggerParticles;

	UPROPERTY(EditAnywhere)
	float ImpulseStrength = 2000.0f;

	UPROPERTY(EditAnywhere)
	float DamageRadius = 1000.0f;

private:
	UFUNCTION()
	void OnTriggered();

	UFUNCTION()
	void DrawCurrentPosition();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle DebugTimerHandle;
};
