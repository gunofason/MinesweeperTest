#pragma once

#include "CoreMinimal.h"

#define LOCTEXT_NAMESPACE "SMinesweeperWidget"

class SMinesweeperWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget)
		: _mWidth(0)
		, _mHeight(0)
		, _mNumMines(0)
		{
		}
		SLATE_ARGUMENT(int32, mWidth)
		SLATE_ARGUMENT(int32, mHeight)
		SLATE_ARGUMENT(int32, mNumMines)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetupBoard(int32 Width, int32 Height, int32 NumMines);
	void Reset();

private:
	int32 mWidth;
	int32 mHeight;
	int32 mNumMines;
	bool bGameOver;
	bool bGameWon;

	TArray<TArray<bool>> mMines;
	TArray<TArray<int32>> mNumbers;
	TArray<TArray<TSharedPtr<SButton>>> mButtons;
	TArray<TArray<bool>> mRevealed;
	TSharedPtr<SGridPanel> mGridPanel;

	const FText mLostTitle = LOCTEXT("LostTitle", "GAME OVER");
	const FText mLostMessage = LOCTEXT("LostMessage", "Sorry, you have found a bomb.");
	const FText mWinTitle = LOCTEXT("WinTitle", "YOU WIN");
	const FText mWinMessage = LOCTEXT("WinMessage", "Congratulations.");

	void InitializeGame();
	void SetupGridPanel();

	void RevealCell(int32 Row, int32 Col);
	FReply OnCellClicked(int32 Row, int32 Col);
	int32 CountAdjacentMines(int32 Row, int32 Col);
	void CheckWinCondition();
	void RevealAllMines();
	FText GetButtonText(int32 Row, int32 Col) const;
	FSlateColor GetButtonColor(int32 Row, int32 Col) const;
};
#undef LOCTEXT_NAMESPACE