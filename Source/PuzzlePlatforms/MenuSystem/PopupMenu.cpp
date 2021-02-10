// Fill out your copyright notice in the Description page of Project Settings.

#include "PopupMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UPopupMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	check(CancelButton != nullptr);
	check(QuitButton != nullptr);
	CancelButton->OnClicked.AddDynamic(this, &UPopupMenu::Cancel);
	QuitButton->OnClicked.AddDynamic(this, &UPopupMenu::Quit);
	return true;
}

void UPopupMenu::Cancel()
{
	Teardown();
}

void UPopupMenu::Quit()
{
	check(MenuInterface != nullptr);
	MenuInterface->GoToMainMenu();
}
