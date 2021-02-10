// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"

#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

APlatformTrigger::APlatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	check(TriggerVolume != nullptr);
	SetRootComponent(TriggerVolume);
}

void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AMovingPlatform* MovingPlatform : PlatformsToTrigger)
	{
		check(MovingPlatform != nullptr);
		MovingPlatform->AddActiveTrigger();
	}
}

void APlatformTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (AMovingPlatform* MovingPlatform : PlatformsToTrigger)
	{
		check(MovingPlatform != nullptr);
		MovingPlatform->RemoveActiveTrigger();
	}
}
