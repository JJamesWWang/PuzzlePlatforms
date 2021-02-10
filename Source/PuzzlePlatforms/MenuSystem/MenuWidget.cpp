// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* Interface)
{
	MenuInterface = Interface;
}

void UMenuWidget::Setup()
{
	bIsFocusable = true;
	AddToViewport();

	UWorld* World = GetWorld();
	check(World != nullptr);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	check(PlayerController != nullptr);

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
	bIsFocusable = false;
	RemoveFromViewport();

	UWorld* World = GetWorld();
	check(World != nullptr);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	check(PlayerController != nullptr);

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	Teardown();
}
