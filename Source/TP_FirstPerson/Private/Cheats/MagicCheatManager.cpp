// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheats/MagicCheatManager.h"

#include "TP_FirstPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_FirstPerson/TP_FirstPersonPlayerController.h"

void UMagicCheatManager::CheatToggleFlyMode()
{
	UE_LOG(LogTemp, Display, TEXT("UMagicCheatManager::CheatEnableFlyMode"));
	UE_LOG(LogTemp, Display, TEXT("Owner: %s"), *GetOuter()->GetName());

	const ATP_FirstPersonPlayerController* PlayerController = Cast<ATP_FirstPersonPlayerController>(GetOuter());
	ATP_FirstPersonCharacter* PlayerCharacter = Cast<ATP_FirstPersonCharacter>(PlayerController->GetPawn());
	UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
	UCapsuleComponent* CapsuleComponent = PlayerCharacter->GetCapsuleComponent();
	
	if(MovementComponent->IsMovingOnGround())
	{
		MovementComponent->NavAgentProps.bCanFly = true;
		MovementComponent->SetMovementMode(MOVE_Flying);
		CapsuleComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}
	else
	{
		MovementComponent->NavAgentProps.bCanFly = false;
		MovementComponent->SetMovementMode(MOVE_Walking);
		CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);	
	}
	
	PlayerCharacter->UpdateNavAgent();
	MovementComponent->StopActiveMovement();
	
	MovementComponent->Deactivate();
	MovementComponent->Activate();
}

void UMagicCheatManager::CheatAddBullets(int32 InBullets)
{
	const APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	APawn* PlayerPawn = playerController->GetPawn();
	ATP_FirstPersonCharacter* PlayerCharacter = Cast<ATP_FirstPersonCharacter>(PlayerPawn);
	PlayerCharacter->OnAmmoPickUp();
}
