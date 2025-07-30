// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WFAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WARFORAPPLE_API AWFAGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
