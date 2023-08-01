// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Components/ProgressBar.h"
#include "UI/STUBaseWidget.h"
#include "STUPlayerHUDWidget.generated.h"

/**
 * 
 */

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnTakeDamage();

	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	FString FormatBullets(int32 BulletsNum) const;

protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	float PercentColorThreshold = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	float PercentColorRisk = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FLinearColor GoodColor = FLinearColor::Yellow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FLinearColor BadColor = FLinearColor::Red;	
	
	virtual void NativeOnInitialized() override;

private:
	void OnHealthChanged(float Health, float HealthDelta);
	void OnNewPawn(APawn* Pawn);
	void UpdateHealthBar();
};
