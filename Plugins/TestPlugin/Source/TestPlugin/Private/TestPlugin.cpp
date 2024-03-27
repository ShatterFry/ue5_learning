// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestPlugin.h"

#include "ContentBrowserDelegates.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "FTestPluginModule"

void FTestPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBMenuExtension();
}

void FTestPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#pragma region ContentBrowserMenuExtension

void FTestPluginModule::InitCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders = 
	ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestPluginModule, TestPlugin)