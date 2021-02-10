// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "UObject/ConstructorHelpers.h"

const static FName KSESSION_NAME = TEXT("SessionName");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	check(MainMenuBPClass.Class != nullptr);
	MainMenuClass = MainMenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> PopupMenuBPClass(TEXT("/Game/MenuSystem/WBP_PopupMenu"));
	check(PopupMenuBPClass.Class != nullptr);
	PopupMenuClass = PopupMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	check(OSS != nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Found subsystem: %s"), *OSS->GetSubsystemName().ToString());
	SessionInterface = OSS->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnHostSessionCompleted);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
			this, &UPuzzlePlatformsGameInstance::OnDestroySessionCompleted);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsCompleted);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionCompleted);
	}

	if (GEngine != nullptr)
	{
		GEngine->NetworkFailureEvent.AddUObject(this, &UPuzzlePlatformsGameInstance::OnNetworkFailed);
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	check(MainMenu != nullptr);
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadPopupMenu()
{
	UMenuWidget* PopupMenu = CreateWidget<UMenuWidget>(this, PopupMenuClass);
	check(PopupMenu != nullptr);
	PopupMenu->Setup();
	PopupMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::GoToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	check(PlayerController != nullptr);
	PlayerController->ClientTravel(TEXT("/Game/MenuSystem/MainMenu"), ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Host(FString SessionName)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
	else
	{
		CreateSession(SessionName);
	}
}

void UPuzzlePlatformsGameInstance::CreateSession(FString SessionName)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	check(OSS != nullptr);

	FOnlineSessionSettings SessionSettings;
	FName SubsystemName = OSS->GetSubsystemName();
	if (SubsystemName == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else if (SubsystemName == "Steam")
	{
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinViaPresence = true;
	}
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinInProgress = false;
	if (!SessionName.IsEmpty())
	{
		SessionSettings.Set(KSESSION_NAME, SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
}

void UPuzzlePlatformsGameInstance::RefreshSessionList()
{
	FindSessions();
}

void UPuzzlePlatformsGameInstance::FindSessions()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding sessions..."));
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::JoinIP(const FString& IPAddress)
{
	UEngine* Engine = GetEngine();
	check(Engine != nullptr);
	Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s..."), *IPAddress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	check(PlayerController != nullptr);
	PlayerController->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::JoinSession(uint32 Index)
{
	check(SessionInterface.IsValid());
	check(SessionSearch.IsValid());
	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	check(SessionInterface.IsValid());
	SessionInterface->StartSession(NAME_GameSession);
}

void UPuzzlePlatformsGameInstance::OnHostSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}
	UEngine* Engine = GetEngine();
	check(Engine != nullptr);
	Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting..."));

	UWorld* World = GetWorld();
	check(World != nullptr);
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}
	CreateSession();
}

void UPuzzlePlatformsGameInstance::OnFindSessionsCompleted(bool bWasSuccessful)
{
	if (!bWasSuccessful || !SessionSearch.IsValid())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found sessions."));
	TArray<FSessionData> SessionDataList;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (SearchResult.IsValid())
		{
			FSessionData Data;
			FString SessionName;
			Data.Name =
				SearchResult.Session.SessionSettings.Get(KSESSION_NAME, SessionName) ? SessionName : SearchResult.GetSessionIdStr();
			Data.HostUsername = SearchResult.Session.OwningUserName;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			SessionDataList.Add(Data);
			UE_LOG(LogTemp, Warning, TEXT("Name : %s, HostUsername : %s, MaxPlayers: %d, CurrentPlayers: %d"), *Data.Name,
				*Data.HostUsername, Data.MaxPlayers, Data.CurrentPlayers);
		}
	}
	MainMenu->SetSessionList(SessionDataList);
}

void UPuzzlePlatformsGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	check(SessionInterface.IsValid());

	UEngine* Engine = GetEngine();
	check(Engine != nullptr);
	Engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s..."), *SessionName.ToString()));

	FString ConnectURL;
	bool HasJoinedSession = SessionInterface->GetResolvedConnectString(SessionName, ConnectURL);
	if (!HasJoinedSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session failed to join. Connect info: %s. Error code %d."), Result);
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	check(PlayerController != nullptr);
	PlayerController->ClientTravel(ConnectURL, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::OnNetworkFailed(
	UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	GoToMainMenu();
}
