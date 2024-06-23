// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TP_FirstPersonCharacter.generated.h"

class USaveGame;
class UMagicHealthComponent;
class UMagicHUD;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UMagicPauseMenu;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATP_FirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(Config)
	int32 InitialHealth = 100;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(BlueprintReadOnly, Category = Magic, meta=(AllowPrivateAccess = "true"))
	UMagicHealthComponent* HealthComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FlightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SaveGameAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LoadGameAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseGameAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<USaveGame> SaveGameClass;
	
public:
	ATP_FirstPersonCharacter();

	DECLARE_DELEGATE(FOnGamePauseRequested)
	FOnGamePauseRequested OnGamePauseRequestedDelegate;

	UFUNCTION(BlueprintCallable)
	void OnGamePauseRequested();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable)
	int32 GetBulletsCount();

	UFUNCTION(BlueprintCallable)
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable)
	int32 GetInitialHealth() const;

	UFUNCTION(BlueprintCallable)
	UMagicHUD* GetHUD();
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void SaveGame(const FInputActionValue& Value);
	void LoadGame(const FInputActionValue& Value);

	void Fly(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable)
	void SetHUD(UMagicHUD* InHUD);

	

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void OnWeaponFired();

	DECLARE_DELEGATE(FOnWeaponChanged)
	FOnWeaponChanged OnWeaponChangedDelegate;

	UFUNCTION()
	void OnWeaponChanged(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnAmmoPickUp();

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMagicHUD> mMagicHUD;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMagicPauseMenu> mPauseMenu;

private:
	int32 BulletCount = 0;

	FString SaveSlotName = TEXT("SaveGameSlot");
	int32 SaveSlotIndex = 0;

	UFUNCTION(BlueprintCallable)
	void BPCallableTestFunc();
};

