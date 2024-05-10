// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "TP_FirstPersonCharacter.h"
#include "TP_FirstPersonProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MagicThrowable.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr || Character->GetBulletsCount() <= 0)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<ATP_FirstPersonProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	Character->OnWeaponFired();
}

void UTP_WeaponComponent::Throw()
{
	UE_LOG(LogTemp, Display, TEXT("WeaponComponent - Throwing a grenade!"));

	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if(GrenadeProjectileClass)
	{
		if(UWorld* const World = GetWorld())
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// TODO: Draw debug spheres on projectile path
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AMagicThrowable>(GrenadeProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}

void UTP_WeaponComponent::ThrowAiming()
{
	UE_LOG(LogTemp, Display, TEXT("TP_WeaponComponent ThrowAiming"));
	
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	
	float ThrowableInitialSpeed = 1000.0f;
	
	FVector LaunchVelocity = GetGrenadeSpawnRotation().Vector();
	LaunchVelocity  *= ThrowableInitialSpeed;
	
	FPredictProjectilePathParams PredictProjectilePathParams = FPredictProjectilePathParams(10.0f, GetGrenadeSpawnLocation(),
		LaunchVelocity, 10.0f);
	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(this, PredictProjectilePathParams, PredictResult);
	
	for(const FPredictProjectilePathPointData& PointData : PredictResult.PathData)
	{
		UE_LOG(LogTemp, Display, TEXT("Location: %s Velocity: %s Time: %f"), *PointData.Location.ToString(),
			*PointData.Velocity.ToString(), PointData.Time);
		DrawDebugSphere(GetWorld(), PointData.Location, 10.0f, 5, FColor::Red, false, 10.0f);
	}
}

void UTP_WeaponComponent::AttachWeapon(ATP_FirstPersonCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (Character == nullptr || Character->GetHasRifle())
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);

			// Throw
			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Throw);

			// Throw Aiming
			EnhancedInputComponent->BindAction(ThrowAimingAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::ThrowAiming);

			// Weapon Change
			EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::WeaponChange);
			EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Triggered, Character, &ATP_FirstPersonCharacter::OnWeaponChanged);

			EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::WeaponChange);
			EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::WeaponChange);
			EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Canceled, this, &UTP_WeaponComponent::WeaponChange);
		}
	}

	
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

FVector UTP_WeaponComponent::GetGrenadeSpawnLocation()
{
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + GetGrenadeSpawnRotation().RotateVector(MuzzleOffset);
	return SpawnLocation;
}

FRotator UTP_WeaponComponent::GetGrenadeSpawnRotation()
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	return SpawnRotation;
}

void UTP_WeaponComponent::WeaponChange(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("TP_WeaponComponent WeaponChange() Code"));
	//FVector2D LookAxisVector = Value.Get<FVector2D>();
	float WeaponChnageValue = Value.Get<float>();
	UE_LOG(LogTemp, Display, TEXT("Value = %f"), WeaponChnageValue);
}

