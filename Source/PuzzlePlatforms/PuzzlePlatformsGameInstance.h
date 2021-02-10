// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"

#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(Exec)
	virtual void Host(FString SessionName = TEXT("")) override;
	UFUNCTION(Exec)
	virtual void JoinIP(const FString& IPAddress) override;
	UFUNCTION(Exec)
	virtual void JoinSession(uint32 Index) override;
	UFUNCTION(Exec)
	virtual void StartSession();
	UFUNCTION(Exec)
	virtual void RefreshSessionList() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadPopupMenu();

	UFUNCTION(Exec)
	virtual void GoToMainMenu() override;

	UFUNCTION()
	void OnNetworkFailed(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	class UMainMenu* MainMenu = nullptr;
	TSubclassOf<UUserWidget> PopupMenuClass;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch = nullptr;

	void CreateSession(FString SessionName = FString());
	void FindSessions();
	void OnHostSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsCompleted(bool bWasSuccessful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
