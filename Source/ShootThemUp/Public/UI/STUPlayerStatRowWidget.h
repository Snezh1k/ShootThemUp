// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "STUPlayerStatRowWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayerNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* KillsTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DeathsTextBlock;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TeamTextBlock;

	UPROPERTY(meta=(BindWidget))
	UImage* PlayerIndicateImage;

	UPROPERTY(meta=(BindWidget))
	UImage* TeamImage;
	
public:
	void SetPlayerName(const FText& Text);
	void SetKills(const FText& Text);
	void SetDeaths(const FText& Text);
	void SetTeam(const FText& Text);
	void SetPlayerIndicateVisibility(bool Visible);
	void SetTeamColor(const FLinearColor& Color);
};
