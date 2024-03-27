// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetActionUtility.h"
#include "Blueprint/UserWidget.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "MyAssetActionUtility.generated.h"

UCLASS()
class MYMODULETEST_API UMyAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()
	
	void GiveFeedback(const FString& Message, int32 Counter);

	void PrintToScreen(const FString& DebugMessage, FColor DisplayColor);

	UFUNCTION(CallInEditor)
	void CheckPowerOfTwo();

	UFUNCTION(CallInEditor)
	void AddPrefixes();

	const TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
				
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("S_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
				
		{UUserWidget::StaticClass(), TEXT("UW_")},
		{UMorphTarget::StaticClass(), TEXT("MT_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")},
	};
};
