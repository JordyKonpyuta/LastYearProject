// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Character/Players.h"
#include "Controler/CustomController.h"
#include "GameMode/WFAGameInstance.h"
#include "GameMode/WFAGameMode.h"
#include "GameMode/WFAGameState.h"
#include "GameMode/WFAPlayerState.h"
#include "WFALibrary.generated.h"

class AActor;
class APlayers;
class ACustomController;
class AWFAPlayerState;
class AWFAGameState;
class AWFAGameMode;
class UWFAGameInstance;

/**
 * 
 */
UCLASS(Blueprintable)
class WARFORAPPLE_API UWFALibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "WFALibrary")
	static void TeleportPlayerAtActor(AActor* Player, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "WFALibrary")
	static void TeleportPlayerAtLocation(AActor* Player, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static APlayers* GetRPGPlayer();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static ACustomController* GetRPGController();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static AWFAPlayerState* GetRPGPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static AWFAGameState* GetRPGGameState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static AWFAGameMode* GetRPGGameMode();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static UWFAGameInstance* GetRPGGameInstance();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WFALibrary")
	static bool DoesSaveGameExist();
};
