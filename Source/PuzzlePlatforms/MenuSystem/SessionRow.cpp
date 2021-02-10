// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionRow.h"

#include "Components/Button.h"
#include "MainMenu.h"

bool USessionRow::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	check(SessionButton != nullptr);
	SessionButton->OnClicked.AddDynamic(this, &USessionRow::OnClicked);
	return true;
}

void USessionRow::Setup(class UMainMenu* ParentWidget, uint32 SelectedIndex)
{
	Index = SelectedIndex;
	Parent = ParentWidget;
}

void USessionRow::OnClicked()
{
	check(Parent != nullptr);
	Parent->SetSelectedIndex(Index);
	UE_LOG(LogTemp, Warning, TEXT("Set selected index to %d"), Index);
}
