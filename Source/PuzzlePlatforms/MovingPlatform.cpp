// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers += 1;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers -= 1;
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority() && ActiveTriggers >= RequiredTriggers)
	{
		MoveTowardsTarget(DeltaTime);
		InvertDirectionIfPastTarget();
	}
}

void AMovingPlatform::MoveTowardsTarget(float DeltaTime)
{
	FVector Location = GetActorLocation();
	FVector MovementDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
	Location += MovementDirection * Speed * DeltaTime;
	SetActorLocation(Location);
}

void AMovingPlatform::InvertDirectionIfPastTarget()
{
	FVector Location = GetActorLocation();
	float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).SizeSquared();
	float TraveledLength = (Location - GlobalStartLocation).SizeSquared();
	if (TraveledLength >= JourneyLength)
	{
		Swap<FVector>(GlobalStartLocation, GlobalTargetLocation);
	}
}
