// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "PopupMenu.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UPopupMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void Cancel();
	UFUNCTION()
	void Quit();
};
