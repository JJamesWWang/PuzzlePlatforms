// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "SessionRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> SessionRowWBPClass(TEXT("/Game/MenuSystem/WBP_SessionRow"));
	check(SessionRowWBPClass.Class != nullptr);
	SessionRowClass = SessionRowWBPClass.Class;
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	check(HostUIButton != nullptr);
	check(JoinUIButton != nullptr);
	check(ConnectButton != nullptr);
	check(GoBackToMainMenuButton != nullptr);
	check(QuitButton != nullptr);
	check(HostButton != nullptr);
	HostUIButton->OnClicked.AddDynamic(this, &UMainMenu::HostUI);
	JoinUIButton->OnClicked.AddDynamic(this, &UMainMenu::JoinUI);
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::Connect);
	GoBackToMainMenuButton->OnClicked.AddDynamic(this, &UMainMenu::GoBackToMainMenu);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::Quit);
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::Host);
	return true;
}

void UMainMenu::Host()
{
	check(MenuInterface != nullptr);
	FString SessionName = SessionNameTextBox->Text.ToString();
	MenuInterface->Host(SessionName);
}

void UMainMenu::HostUI()
{
	check(MainMenuSwitcher != nullptr);
	check(HostAGameMenu != nullptr);
	MainMenuSwitcher->SetActiveWidget(HostAGameMenu);
}

void UMainMenu::JoinUI()
{
	check(MainMenuSwitcher != nullptr);
	check(JoinAGameMenu != nullptr);
	MainMenuSwitcher->SetActiveWidget(JoinAGameMenu);
	check(MenuInterface != nullptr);
	MenuInterface->RefreshSessionList();
}

void UMainMenu::SetSessionList(TArray<FSessionData> SessionDataList)
{
	check(SessionRowClass != nullptr);
	SessionList->ClearChildren();

	uint32 SelectIndex = 0;
	for (FSessionData SessionData : SessionDataList)
	{
		USessionRow* SessionRow = CreateWidget<USessionRow>(this, SessionRowClass);
		SessionRow->SessionNameText->SetText(FText::FromString(SessionData.Name));
		SessionRow->HostNameText->SetText(FText::FromString(SessionData.HostUsername));
		SessionRow->ConnectionsText->SetText(
			FText::FromString(FString::Printf(TEXT("%d/%d"), SessionData.CurrentPlayers, SessionData.MaxPlayers)));
		SessionRow->Setup(this, SelectIndex++);
		SessionList->AddChild(SessionRow);
	}
}

void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateSessionRows();
}

void UMainMenu::UpdateSessionRows()
{
	for (int RowIndex = 0; RowIndex < SessionList->GetChildrenCount(); ++RowIndex)
	{
		USessionRow* SessionRow = Cast<USessionRow>(SessionList->GetChildAt(RowIndex));
		if (SessionRow)
		{
			SessionRow->Selected = SelectedIndex.IsSet() && RowIndex == SelectedIndex.GetValue();
		}
	}
}

void UMainMenu::Connect()
{
	if (IPAddressTextBox->bIsEnabled)
	{
		check(IPAddressTextBox != nullptr);
		check(MenuInterface != nullptr);
		FString IPAddress = IPAddressTextBox->GetText().ToString();
		MenuInterface->JoinIP(IPAddress);
	}
	else
	{
		check(MenuInterface != nullptr);
		if (SelectedIndex.IsSet())
		{
			MenuInterface->JoinSession(SelectedIndex.GetValue());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected Index isn't set."));
		}
	}
}

void UMainMenu::GoBackToMainMenu()
{
	check(MainMenuSwitcher != nullptr);
	check(MainMenu != nullptr);
	MainMenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::Quit()
{
	UWorld* World = GetWorld();
	check(World != nullptr);
	APlayerController* PlayerController = World->GetFirstPlayerController();
	check(PlayerController != nullptr);
	PlayerController->ConsoleCommand(TEXT("quit"));
}