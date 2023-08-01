// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGoToMenuWidget.h"

#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu);
	}
}

void USTUGoToMenuWidget::OnGoToMenu()
{
	if(!GetWorld()) return;
	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if(!STUGameInstance) return;
	if(STUGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Menu Name is none!"))
		return;
	}
	const FName StartUpLevelName = "Test";
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}
