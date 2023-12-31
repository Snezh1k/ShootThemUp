// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/STUMenuWidget.h"
#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}
	if(QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}
	InitLevelItems();
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if(Animation != HideAnimation) return;
	const auto STUGameInstance = GetGameInstance();
	if(!STUGameInstance) return;
	
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

void USTUMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetGameInstance();
	if(!STUGameInstance) return;

	checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data is empty!"))

	if(!LevelItemBox) return;
	LevelItemBox->ClearChildren();

	for(auto LevelData : STUGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if(!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);
		LevelItemBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}
	if(STUGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel());
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetGameInstance();
	if(!STUGameInstance) return;

	STUGameInstance->SetStartupLevel(Data);
	for(auto LevelItemWidget : LevelItemWidgets)
	{
		if(LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

USTUGameInstance* USTUMenuWidget::GetGameInstance() const
{
	if(!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}
