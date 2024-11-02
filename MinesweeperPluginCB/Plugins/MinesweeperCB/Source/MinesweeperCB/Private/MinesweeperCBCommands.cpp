// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperCBCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperCBModule"

void FMinesweeperCBCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MinesweeperCB", "Execute MinesweeperCB action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
