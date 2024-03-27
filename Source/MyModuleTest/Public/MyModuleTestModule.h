// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "Modules/ModuleInterface.h"

MYMODULETEST_API DECLARE_LOG_CATEGORY_EXTERN(LogMyModuleTest, Display, All);

class FMyModuleTestModule : public IModuleInterface
{
public:
	// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void InitCBMenuExtension();
};