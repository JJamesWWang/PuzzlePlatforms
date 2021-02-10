// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "GameMapsSettings.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	Players += 1;
	if (Players >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lobby has 3+ players!"));
		FTimerHandle TransitionHandle;
		GetWorldTimerManager().SetTimer(TransitionHandle, this, &ALobbyGameMode::Transition, 10);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	Players -= 1;
}

void ALobbyGameMode::Transition()
{
	UWorld* World = GetWorld();
	check(World != nullptr);
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	UPuzzlePlatformsGameInstance* GameInstance = GetGameInstance<UPuzzlePlatformsGameInstance>();
	if (GameInstance)
	{
		GameInstance->StartSession();
	}
}
