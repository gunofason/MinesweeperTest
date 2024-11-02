#pragma once

#include "CoreMinimal.h"
#include "MinesweeperWidgetCB.h"

#define LOCTEXT_NAMESPACE "SMinesweeperWidget"
class MinesweeperCBSlate
{
public:
	TSharedRef<SDockTab> GetDockTab();

private:
	TSharedRef<SDockTab> CreateDockTab();
	void OnClose(TSharedRef<SDockTab> InTab);

	void SetMinesweeperWidth(int32 Width);
	void SetMinesweeperHeight(int32 Height);
	void SetMinesweeperNumMines(int32 NumMines);
	FReply SetupBoard();

private:
	int32 mWidth = 8;
	int32 mHeight = 8;
	int32 mNumMines = 8;
	
	const FText mInvalidParams = FText(LOCTEXT("InvalidParams", "You need more tiles for that many bombs."));
	const FText mInvalidParamsTitle = FText(LOCTEXT("InvalidParamsTitle", "Invalid Params"));

	TSharedPtr<SMinesweeperWidget> mSMinesweeperWidget;
	
};
#undef LOCTEXT_NAMESPACE