// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/WFA_Item.h"
#include "WFAPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WARFORAPPLE_API AWFAPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FWFA_Inventory> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FWFA_Inventory> Equipment = {FWFA_Inventory(), FWFA_Inventory(), FWFA_Inventory()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FWFA_Quest> Quests = {FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<int> QuestsStages = {0,0,0,0,0,5,0,5,0,5,0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<bool> CompletedQuests = {false, false, false, false, false, false, false, false, false, false, false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<bool> UnlockedInvocations = {false, false, false, false, false, false, false, false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 PlayerSkillPoints = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 PlayerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 PlayerXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 PlayerXPToNextLevel = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 AppleJuice = 100;

	// Getters //
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Info")
	FString GetPlayerUserName() const { return PlayerName; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Info")
	int32 GetPlayerLevel() const { return PlayerLevel; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Info")
	int32 GetPlayerXP() const { return PlayerXP; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Info")
	int32 GetPlayerXPToNextLevel() const { return PlayerXPToNextLevel; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Info")
	int32 GetAppleJuice() const { return AppleJuice; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Info")
	int32 GetPlayerSkillPoints() const { return PlayerSkillPoints; }
	
	// Setters //
	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void SetPlayerUserName(FString NewPlayerName);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void AddPlayerLevel(int32 PlayerLevelToAdd);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void SetPlayerLevel(int32 NewPlayerLevel);
	
	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void AddPlayerXP(int32 PlayerXPToAdd);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void SetPlayerXP(int32 NewPlayerXP);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void SetPlayerXPToNextLevel(int32 NewPlayerXPToNextLevel);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void AddAppleJuice(int32 NewAppleJuice);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void RemoveAppleJuice(int32 NewAppleJuice);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void SetAppleJuice(int32 NewAppleJuice);

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void AddPlayerSkillPoints(int32 NewSkillPoints) {PlayerSkillPoints += NewSkillPoints; }

	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void SetPlayerSkillPoints(int32 NewSkillPoints);


	// Player State Functions //
	UFUNCTION(BlueprintCallable, Category = "Player - Info")
	void CheckLevelUp();


	// Player State Callback //
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Player - Info")
	void OnPlayerLevelUp();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Player - Info")
	void OnPlayerXPChange();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Player - Info")
	void OnAppleJuiceChange();

	// Inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddInventory(FWFA_Inventory ItemToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveInventory(int Index);
	
	void RemoveInventory(FWFA_Inventory ItemToRemove);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int SearchInInventory(FString nameToFind);

	// UI
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
	void PlayRespawnAnimation();

	// Base Functions //
	virtual void BeginPlay() override;
};