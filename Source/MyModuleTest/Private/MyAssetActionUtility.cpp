// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetActionUtility.h"
#include "EditorUtilityLibrary.h"

void UMyAssetActionUtility::GiveFeedback(const FString& Message, int32 Counter)
{
	const FString& DebugMessage = FString::Printf(TEXT("User Feedback: %s %d"), *Message, Counter);
	UE_LOG(LogTemp, Display, TEXT("%s"), *DebugMessage);
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, DebugMessage);
}

void UMyAssetActionUtility::PrintToScreen(const FString& DebugMessage, FColor DisplayColor)
{
	GEngine->AddOnScreenDebugMessage(0, 5.0f, DisplayColor, DebugMessage);
}

void UMyAssetActionUtility::CheckPowerOfTwo()
{
	
}

void UMyAssetActionUtility::AddPrefixes()
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;
	
	for(UObject* SelectedObject : SelectedObjects)
	{
		if(ensure(SelectedObject))
		{
			const FString* Prefix = PrefixMap.Find(SelectedObject->GetClass());
			if(ensure(Prefix) && !Prefix->Equals(TEXT("")))
			{
				FString OldName = SelectedObject->GetName();
				if(OldName.StartsWith(*Prefix))
				{
					continue;
				}
				FString NewName = *Prefix + OldName;
				UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
				++Counter;
			}
			else
			{
				PrintToScreen(FString::Printf(TEXT("Couldn't find prefix for class %s"),
					*SelectedObject->GetClass()->GetName()), FColor::Red);
			}
		}
	}

	GiveFeedback(TEXT("Added prefix to"), Counter);
}
