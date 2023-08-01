// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "STUHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	float PercentVisibleThreshold = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FLinearColor GoodColor = FLinearColor::Yellow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FLinearColor BadColor = FLinearColor::Red;	
};
