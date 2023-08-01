// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUPlayerStatRowWidget.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if(!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
	if(!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if(!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if(!TeamTextBlock) return;
	TeamTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerIndicateVisibility(bool Visible)
{
	if(!PlayerIndicateImage) return;
	PlayerIndicateImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if(!TeamImage) return;
	TeamImage->SetColorAndOpacity(Color);
}
