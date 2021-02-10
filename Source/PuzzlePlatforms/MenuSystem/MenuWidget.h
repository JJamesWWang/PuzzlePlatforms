// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "MenuInterface.h"

#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	void SetMenuInterface(IMenuInterface* Interface);

protected:
	IMenuInterface* MenuInterface = nullptr;
};