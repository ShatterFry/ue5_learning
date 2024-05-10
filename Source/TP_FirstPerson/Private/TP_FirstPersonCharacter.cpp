// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_FirstPersonCharacter.h"
#include "TP_FirstPersonProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MagicHealthComponent.h"
#include "MagicHUD.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/MagicLocalPlayerSaveGame.h"
#include "Globals/GlobalGameStructs.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATP_FirstPersonCharacter

void ATP_FirstPersonCharacter::OnWeaponFired()
{
	UE_LOG(LogTemp, Display, TEXT("ATP_FirstPersonCharacter::OnWeaponFired"));
	--BulletCount;
	UE_LOG(LogTemp, Display, TEXT("BulletCount = %d"), BulletCount);
	mMagicHUD->OnBulletsCountChanged(BulletCount);
}

void ATP_FirstPersonCharacter::OnAmmoPickUp()
{
	UE_LOG(LogTemp, Display, TEXT("Character OnAmmoPickUp"));

	FSoftObjectPath TablePath = FSoftObjectPath("/Game/Tables/GameplayDataTable.GameplayDataTable");
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *TablePath.ToString(), nullptr, LOAD_None, nullptr);
	check(DataTable);
	FMagicAmmoGameplayDataTableRow* RowData = DataTable->FindRow<FMagicAmmoGameplayDataTableRow>(FName("AmmoPickUp"), TEXT("DataTest"));

	BulletCount += RowData->BulletsCapacity;
	mMagicHUD->OnBulletsCountChanged(BulletCount);
}

void ATP_FirstPersonCharacter::OnWeaponChanged(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Character OnWeaponChanged"));
	OnWeaponChangedDelegate.ExecuteIfBound();
}

ATP_FirstPersonCharacter::ATP_FirstPersonCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	HealthComponent = CreateDefaultSubobject<UMagicHealthComponent>(TEXT("CharacterHealth"));
	OnTakeRadialDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakeRadialDamage);
	OnTakeAnyDamage.AddUniqueDynamic(HealthComponent, &UMagicHealthComponent::OnTakeAnyDamage);
	UE_LOG(LogTemp, Display, TEXT("Constructor Character InitialHealth: %d"), InitialHealth);
}

void ATP_FirstPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("BeginPlay Character InitialHealth: %d"), InitialHealth);
	HealthComponent->SetInitialHealth(InitialHealth);
	HealthComponent->SetHealth(InitialHealth);
	
	const FText HealthText = FText::FromString(FString::Printf(TEXT("Health: %d"), GetHealth()));
	mMagicHUD->SetHealthText(HealthText);
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ATP_FirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Look);

		// SaveGame
		EnhancedInputComponent->BindAction(SaveGameAction, ETriggerEvent::Triggered, this,
			&ATP_FirstPersonCharacter::SaveGame);

		// LoadGme
		EnhancedInputComponent->BindAction(LoadGameAction, ETriggerEvent::Triggered, this,
			&ATP_FirstPersonCharacter::LoadGame);

		EnhancedInputComponent->BindAction(FlightAction, ETriggerEvent::Triggered, this,
			&ATP_FirstPersonCharacter::Fly);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATP_FirstPersonCharacter::SetHUD(UMagicHUD* InHUD)
{
	mMagicHUD = InHUD;
	OnWeaponChangedDelegate.BindUObject(mMagicHUD, &UMagicHUD::OnWeaponChanged);
}

int32 ATP_FirstPersonCharacter::GetBulletsCount()
{
	return BulletCount;
}

int32 ATP_FirstPersonCharacter::GetHealth() const
{
	return HealthComponent->GetHealth();
}

int32 ATP_FirstPersonCharacter::GetInitialHealth() const
{
	return InitialHealth;
}

UMagicHUD* ATP_FirstPersonCharacter::GetHUD()
{
	return mMagicHUD;
}


void ATP_FirstPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATP_FirstPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATP_FirstPersonCharacter::SaveGame(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("SaveGame"));
	
	if(USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveSlotIndex))
	{
		UMagicLocalPlayerSaveGame* MagicSaveGame = CastChecked<UMagicLocalPlayerSaveGame>(SaveGame);
		if(MagicSaveGame->IsSaveGameInProgress())
		{
			return;
		}
	}
	
	UMagicLocalPlayerSaveGame* SaveGame = CastChecked<UMagicLocalPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UMagicLocalPlayerSaveGame::StaticClass()));
	FVector ActorLocation = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("ActorLocation: %s"), *ActorLocation.ToString());
	SaveGame->SetActorLocation(ActorLocation);
	SaveGame->SetActorRotation(GetActorRotation());
	SaveGame->SetSaveGameInProgress(true);
	
	UGameplayStatics::AsyncSaveGameToSlot(SaveGame, SaveSlotName, SaveSlotIndex, FAsyncSaveGameToSlotDelegate::CreateLambda([&](const FString& SlotName, const int32 UserIndex, bool bSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("Save completed! %s %d %d"), *SlotName, UserIndex, bSuccessful);
		SaveGame->SetSaveGameInProgress(false);
	}));
}

void ATP_FirstPersonCharacter::LoadGame(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("LoadGame"));

	bool SaveExists = UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveSlotIndex);

	if(USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveSlotIndex))
	{
		const UMagicLocalPlayerSaveGame* MagicSaveGame = CastChecked<UMagicLocalPlayerSaveGame>(SaveGame);
		const FVector ActorLocation = MagicSaveGame->GetActorLocation();
		UE_LOG(LogTemp, Display, TEXT("ActorLocation = %s"), *ActorLocation.ToString());
		TeleportTo(ActorLocation, MagicSaveGame->GetActorRotation());
	}
}

void ATP_FirstPersonCharacter::Fly(const FInputActionValue& Value)
{
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FRotator ControlRotation = PlayerController->GetControlRotation();
	APawn* PlayerPawn = PlayerController->GetPawn();
	ATP_FirstPersonCharacter* PlayerCharacter = Cast<ATP_FirstPersonCharacter>(PlayerPawn);
	
	const FVector UpVector = GetActorUpVector();
	const FVector2D MoveDirection = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Display, TEXT("Fly input: %s"), *MoveDirection.ToString());
	AddMovementInput(UpVector, MoveDirection.X);
}

void ATP_FirstPersonCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ATP_FirstPersonCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ATP_FirstPersonCharacter::BPCallableTestFunc()
{
	UE_LOG(LogTemp, Display, TEXT("void ATP_FirstPersonCharacter::BPCallableTestFunc() BEGIN"));
	FDebug::DumpStackTraceToLog(ELogVerbosity::Display);
	UE_LOG(LogTemp, Display, TEXT("void ATP_FirstPersonCharacter::BPCallableTestFunc() END"));
}



