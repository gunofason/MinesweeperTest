#include "MinesweeperWidgetCB.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SMinesweeperWidget"

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	mWidth = InArgs._mWidth;
	mHeight = InArgs._mHeight;
	mNumMines = InArgs._mNumMines;

	SetupGridPanel();
}

void SMinesweeperWidget::SetupBoard(int32 Width, int32 Height, int32 NumMines)
{
	mWidth = Width;
	mHeight = Height;
	mNumMines = NumMines;

	SetupGridPanel();
}

void SMinesweeperWidget::InitializeGame()
{
	Reset();

	if (mHeight < 1 || mWidth < 1 || mNumMines < 1) { return; }

	// Initialize arrays
	mMines.SetNum(mHeight);
	mNumbers.SetNum(mHeight);
	mRevealed.SetNum(mHeight);
	for (int32 i = 0; i < mHeight; i++)
	{
		mMines[i].SetNum(mWidth);
		mNumbers[i].SetNum(mWidth);
		mRevealed[i].SetNum(mWidth);
	}

	// Place mines randomly
	int32 MinesPlaced = 0;
	while (MinesPlaced < mNumMines)
	{
		int32 Row = FMath::RandRange(0, mHeight - 1);
		int32 Col = FMath::RandRange(0, mWidth - 1);

		if (!mMines[Row][Col])
		{
			mMines[Row][Col] = true;
			MinesPlaced++;
		}
	}

	// Calculate numbers
	for (int32 Row = 0; Row < mHeight; Row++)
	{
		for (int32 Col = 0; Col < mWidth; Col++)
		{
			if (!mMines[Row][Col])
			{
				mNumbers[Row][Col] = CountAdjacentMines(Row, Col);
			}
		}
	}
}

FReply SMinesweeperWidget::OnCellClicked(int32 Row, int32 Col)
{
	if (!bGameOver && !mRevealed[Row][Col])
	{
		if (mMines[Row][Col])
		{
			bGameOver = true;
			RevealAllMines();
			FMessageDialog::Open(EAppMsgType::Ok, mLostMessage, mLostTitle);
		}
		else
		{
			RevealCell(Row, Col);
			CheckWinCondition();
		}
	}
	return FReply::Handled();
}

void SMinesweeperWidget::RevealCell(int32 Row, int32 Col)
{
	if (Row < 0 || Row >= mHeight || Col < 0 || Col >= mWidth || mRevealed[Row][Col])
	{
		return;
	}

	mRevealed[Row][Col] = true;

	if (mNumbers[Row][Col] == 0)
	{
		// Reveal adjacent cells for empty spaces
		for (int32 i = -1; i <= 1; i++)
		{
			for (int32 j = -1; j <= 1; j++)
			{
				RevealCell(Row + i, Col + j);
			}
		}
	}
}

int32 SMinesweeperWidget::CountAdjacentMines(int32 Row, int32 Col)
{
	int32 Count = 0;
	for (int32 i = -1; i <= 1; i++)
	{
		for (int32 j = -1; j <= 1; j++)
		{
			int32 NewRow = Row + i;
			int32 NewCol = Col + j;
			if (NewRow >= 0 && NewRow < mHeight && NewCol >= 0 && NewCol < mWidth)
			{
				if (mMines[NewRow][NewCol])
				{
					Count++;
				}
			}
		}
	}
	return Count;
}

void SMinesweeperWidget::CheckWinCondition()
{
	for (int32 Row = 0; Row < mHeight; Row++)
	{
		for (int32 Col = 0; Col < mWidth; Col++)
		{
			if (!mMines[Row][Col] && !mRevealed[Row][Col])
			{
				return;
			}
		}
	}
	bGameWon = true;
	bGameOver = true;
	FMessageDialog::Open(EAppMsgType::Ok, mWinMessage, mWinTitle);
}

void SMinesweeperWidget::Reset()
{
	bGameOver = false;
	bGameWon = false;
	
	if (mGridPanel.IsValid())
	{
		mGridPanel->ClearChildren();
	}
	
	mMines.Empty();
	mNumbers.Empty();
	mButtons.Empty();
	mRevealed.Empty();
}

void SMinesweeperWidget::RevealAllMines()
{
	for (int32 Row = 0; Row < mHeight; Row++)
	{
		for (int32 Col = 0; Col < mWidth; Col++)
		{
			if (mMines[Row][Col])
			{
				mRevealed[Row][Col] = true;
			}
		}
	}
}


void SMinesweeperWidget::SetupGridPanel()
{
	InitializeGame();

	if (!mGridPanel.IsValid())
	{
		SAssignNew(mGridPanel, SGridPanel);
	}

	// Create buttons for each cell
	for (int32 Row = 0; Row < mHeight; Row++)
	{
		mButtons.Add(TArray<TSharedPtr<SButton>>());
		for (int32 Col = 0; Col < mWidth; Col++)
		{
			TSharedPtr<SButton> Button = SNew(SButton)
				.OnClicked(this, &SMinesweeperWidget::OnCellClicked, Row, Col)
				.ButtonStyle(FCoreStyle::Get(), "Button")
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(STextBlock)
						.Text_Lambda([this, Row, Col]() { return GetButtonText(Row, Col); })
						.ColorAndOpacity_Lambda([this, Row, Col]() { return GetButtonColor(Row, Col); })
				];

			mButtons[Row].Add(Button);

			mGridPanel->AddSlot(Col, Row)
				[
					SNew(SBox)
						.WidthOverride(40)
						.HeightOverride(40)
						[
							Button.ToSharedRef()
						]
				];
		}
	}

	ChildSlot
		[
			mGridPanel.ToSharedRef()
		];
}

FText SMinesweeperWidget::GetButtonText(int32 Row, int32 Col) const
{
	if (!mRevealed[Row][Col])
	{
		return FText::FromString(TEXT(""));
	}

	if (mMines[Row][Col])
	{
		return FText::FromString(TEXT("X"));
	}

	return mNumbers[Row][Col] > 0 ? FText::Format(LOCTEXT("BombDistance", "{0}"), mNumbers[Row][Col]) : FText::FromString(TEXT(""));
}

FSlateColor SMinesweeperWidget::GetButtonColor(int32 Row, int32 Col) const
{
	if (!mRevealed[Row][Col])
	{
		return FSlateColor(FLinearColor::White);
	}

	if (mMines[Row][Col])
	{
		return FSlateColor(FLinearColor::Red);
	}

	// Different colors for different numbers
	switch (mNumbers[Row][Col])
	{
	case 1: return FSlateColor(FLinearColor::Blue);
	case 2: return FSlateColor(FLinearColor::Green);
	case 3: return FSlateColor(FLinearColor::Red);
	case 4: return FSlateColor(FLinearColor(0.5f, 0.0f, 0.5f));
	case 5: return FSlateColor(FLinearColor(0.5f, 0.0f, 0.0f));
	default: return FSlateColor(FLinearColor::Black);
	}
}
#undef LOCTEXT_NAMESPACE