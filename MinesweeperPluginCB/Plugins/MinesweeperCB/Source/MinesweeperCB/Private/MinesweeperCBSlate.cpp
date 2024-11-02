#include "MinesweeperCBSlate.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FMinesweeperUI"

TSharedRef<SDockTab> MinesweeperCBSlate::GetDockTab()
{
	return CreateDockTab();
}

TSharedRef<SDockTab> MinesweeperCBSlate::CreateDockTab()
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab).OnTabClosed_Raw(this, &MinesweeperCBSlate::OnClose)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().AutoHeight().Padding(0, 4)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("TextBlockWidth","Width:\t")))
					]
					+ SHorizontalBox::Slot().MaxWidth(200).Padding(50, 0)
					[
						SNew(SSpinBox<int32>)
							.MinSliderValue(1)
							.MaxSliderValue(32)
							.Value(mWidth)
						.OnValueChanged_Raw(this, &MinesweeperCBSlate::SetMinesweeperWidth)
					]
				]
				+SVerticalBox::Slot().AutoHeight().Padding(0, 4)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().AutoWidth().VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("TextBlockHeight", "Height:\t")))
					]
					+ SHorizontalBox::Slot().MaxWidth(200).Padding(50, 0)
					[
						SNew(SSpinBox<int32>)
						.MinSliderValue(1)
						.MaxSliderValue(32)
						.Value(mHeight)
						.OnValueChanged_Raw(this, &MinesweeperCBSlate::SetMinesweeperHeight)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0,4)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("TextBlockNumberOfBombs", "Number Of Mines:\t")))
					]
					+ SHorizontalBox::Slot().MaxWidth(200).Padding(50, 0)
					[
						SNew(SSpinBox<int32>)
						.MinSliderValue(1)
						.MaxSliderValue(50)
						.Value(mNumMines)
						.OnValueChanged_Raw(this, &MinesweeperCBSlate::SetMinesweeperNumMines)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0, 4)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().MaxWidth(200)
					[
						SNew(SButton)
						.Text(FText(LOCTEXT("GenerateNewBoard", "Generate new board.")))
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.ForegroundColor(FLinearColor(1, 1, 1))
						.OnClicked_Raw(this, &MinesweeperCBSlate::SetupBoard)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0, 8)
				[
					SAssignNew(mSMinesweeperWidget, SMinesweeperWidget)
				]
			]
		];
}



FReply MinesweeperCBSlate::SetupBoard()
{
	if ( (mWidth * mHeight) < mNumMines)
	{
		FMessageDialog::Open(EAppMsgType::Ok, mInvalidParams, mInvalidParamsTitle);
		return FReply::Handled();
	}
	
	mSMinesweeperWidget->SetupBoard(mWidth, mHeight, mNumMines);
	return FReply::Handled();
}

void MinesweeperCBSlate::SetMinesweeperWidth(int32 Width)
{
	mWidth = Width;
}

void MinesweeperCBSlate::SetMinesweeperHeight(int32 Height)
{
	mHeight = Height;
}

void MinesweeperCBSlate::SetMinesweeperNumMines(int32 NumMines)
{
	mNumMines = NumMines;
}

void MinesweeperCBSlate::OnClose(TSharedRef<SDockTab> InTab)
{
	if (mSMinesweeperWidget.IsValid())
	{
		mSMinesweeperWidget->Reset();
	}

	mSMinesweeperWidget.Reset();
}


#undef LOCTEXT_NAMESPACE