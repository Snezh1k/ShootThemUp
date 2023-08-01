// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/STULevelItemWidget.h"

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;
	if(LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}
	if(LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb);
	}
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{
	if(LevelImage)
	{
		LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor(1.0f, 0.0f, 0.0f, 1.0f) : FLinearColor(1.0f, 1.0f, 1.0f, 0.7f));
	}
}

void USTULevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::USTULevelItemWidget::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::USTULevelItemWidget::OnLevelItemUnhovered);
	}
}

void USTULevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::OnLevelItemHovered()
{
	if(FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USTULevelItemWidget::OnLevelItemUnhovered()
{
	if(FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
