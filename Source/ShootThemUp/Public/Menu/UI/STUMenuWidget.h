// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUGameInstance.h"
#include "STULevelItemWidget.h"
#include "UI/STUBaseWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Sound/SoundCue.h"
#include "STUMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* LevelItemBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* StartGameSound;
	
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
private:
	UPROPERTY()
	TArray<USTULevelItemWidget*> LevelItemWidgets; 
	UFUNCTION()
	void OnStartGame();
	
	UFUNCTION()
	void OnQuitGame();

	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	USTUGameInstance* GetGameInstance() const;
};
