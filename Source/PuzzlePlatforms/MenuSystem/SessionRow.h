// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "SessionRow.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API USessionRow : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* SessionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionNameText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostNameText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionsText = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 SelectedIndex);

private:
	class UMainMenu* Parent;
	uint32 Index;

	UFUNCTION()
	void OnClicked();
};
