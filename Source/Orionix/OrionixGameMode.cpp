// Copyright Epic Games, Inc. All Rights Reserved.

#include "OrionixGameMode.h"
#include "OrionixCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOrionixGameMode::AOrionixGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if(PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AOrionixGameMode::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FVector Location = FVector(0.f, 0.f, 500.f);
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);
	TunnelManager = GetWorld()->SpawnActor<ATunnelManager>(ATunnelManager::StaticClass(), Location, Rotation, SpawnParams);

}
