// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"

class UWidget;
class UButton;
class UWidgetSwitcher;
class UEditableTextBox;

USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void SetSessionList(TArray<FSessionData> SessionData);
	void SetSelectedIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainMenuSwitcher = nullptr;
	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu = nullptr;
	UPROPERTY(meta = (BindWidget))
	UWidget* HostAGameMenu = nullptr;
	UPROPERTY(meta = (BindWidget))
	UWidget* JoinAGameMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* HostUIButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* SessionNameTextBox = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinUIButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IPAddressTextBox = nullptr;
	TOptional<uint32> SelectedIndex;

	TSubclassOf<UUserWidget> SessionRowClass = nullptr;
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SessionList = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ConnectButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* GoBackToMainMenuButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton = nullptr;

	UFUNCTION()
	void HostUI();
	UFUNCTION()
	void JoinUI();
	UFUNCTION()
	void Host();
	UFUNCTION()
	void Connect();
	UFUNCTION(BlueprintCallable)
	void GoBackToMainMenu();
	UFUNCTION()
	void Quit();

	void UpdateSessionRows();
};
