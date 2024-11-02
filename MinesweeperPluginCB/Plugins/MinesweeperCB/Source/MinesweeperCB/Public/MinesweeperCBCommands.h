// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperCBStyle.h"

class FMinesweeperCBCommands : public TCommands<FMinesweeperCBCommands>
{
public:

	FMinesweeperCBCommands()
		: TCommands<FMinesweeperCBCommands>(TEXT("MinesweeperCB"), NSLOCTEXT("Contexts", "MinesweeperCB", "MinesweeperCB Plugin"), NAME_None, FMinesweeperCBStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
