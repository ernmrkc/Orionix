// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Block.h"
#include "FBlockPool.h"
#include "TunnelManager.h"
#include "OrionixGameMode.generated.h"

UCLASS(minimalapi)
class AOrionixGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrionixGameMode();
	ATunnelManager *TunnelManager;

protected:
	virtual void BeginPlay() override;
};



