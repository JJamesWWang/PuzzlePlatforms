// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "MovingPlatform.generated.h"

class UPrimitiveComponent;

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	virtual void Tick(float DeltaTime) override;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	FVector GlobalStartLocation;
	FVector GlobalTargetLocation;

	UPROPERTY(EditAnywhere)
	int32 RequiredTriggers = 0;
	int32 ActiveTriggers = 0;

	void MoveTowardsTarget(float DeltaTime);
	void InvertDirectionIfPastTarget();
};
