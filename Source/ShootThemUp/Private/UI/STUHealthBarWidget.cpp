// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUHealthBarWidget.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
	if(!HealthProgressBar) return;
	
	const auto HealthBarVisibility = (Percent > PercentVisibleThreshold || FMath::IsNearlyZero(Percent)) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	HealthProgressBar->SetVisibility(HealthBarVisibility);
	
	const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

	HealthProgressBar->SetPercent(Percent);
}
