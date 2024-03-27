// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
#include "MagicDestructibleActor.generated.h"

class UMagicHealthComponent;

UCLASS()
class TP_FIRSTPERSON_API AMagicDestructibleActor : public AActor, public IVisualLoggerDebugSnapshotInterface
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Magic, meta=(AllowPrivateAccess = "true"))
	UMagicHealthComponent* HealthComponent;
	
public:	
	// Sets default values for this actor's properties
	AMagicDestructibleActor();

	UFUNCTION()
	void OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

#if ENABLE_VISUAL_LOG
	//~ Begin IVisualLoggerDebugSnapshotInterface interface
	// Adds information about this Actor to the Visual Logger.
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
	//~ End IVisualLoggerDebugSnapshotInterface interface
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
