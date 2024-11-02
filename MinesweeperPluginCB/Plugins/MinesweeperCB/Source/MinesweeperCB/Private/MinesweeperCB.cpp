// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperCB.h"
#include "MinesweeperCBStyle.h"
#include "MinesweeperCBCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName MinesweeperCBTabName("MinesweeperCB");

#define LOCTEXT_NAMESPACE "FMinesweeperCBModule"

void FMinesweeperCBModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperCBStyle::Initialize();
	FMinesweeperCBStyle::ReloadTextures();

	FMinesweeperCBCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperCBCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMinesweeperCBModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperCBModule::RegisterMenus));

	// Create the Minesweeper portion.
	mMinesweeperCBSlate = MakeShared<MinesweeperCBSlate>();

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperCBTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperCBModule::OnSpawnNomad));
}

void FMinesweeperCBModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperCBStyle::Shutdown();

	FMinesweeperCBCommands::Unregister();
}

void FMinesweeperCBModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperCBTabName);
}

TSharedRef<SDockTab> FMinesweeperCBModule::OnSpawnNomad(const FSpawnTabArgs& InTabSpawnArgs)
{
	return mMinesweeperCBSlate->GetDockTab();
}

void FMinesweeperCBModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperCBCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperCBCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperCBModule, MinesweeperCB)