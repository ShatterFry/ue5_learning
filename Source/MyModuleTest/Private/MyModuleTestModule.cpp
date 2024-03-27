// Copyright Epic Games, Inc. All Rights Reserved.

//#include "Modules/ModuleManager.h"
#include "MyModuleTestModule.h"

#include "ContentBrowserModule.h"
#include "Modules/ModuleManager.h"

// IMPLEMENT_MODULE(FDefaultModuleImpl, MyModuleTest);
IMPLEMENT_MODULE(FMyModuleTestModule, ModuleTest);

DEFINE_LOG_CATEGORY(LogMyModuleTest);

void FMyModuleTestModule::StartupModule()
{
	UE_LOG(LogMyModuleTest, Display, TEXT("FMyModuleTestModule::StartupModule"));
	InitCBMenuExtension();
}

void FMyModuleTestModule::ShutdownModule()
{
	UE_LOG(LogMyModuleTest, Display, TEXT("FMyModuleTestModule::ShutdownModule"));
}

void FMyModuleTestModule::InitCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders = 
	ContentBrowserModule.GetAllPathViewContextMenuExtenders();
}
