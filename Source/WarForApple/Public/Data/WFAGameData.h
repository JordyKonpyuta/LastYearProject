// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShaderCompiler.h"
#include "GameFramework/SaveGame.h"
#include "Inventory/WFA_Item.h"
#include "WFAGameData.generated.h"

/**
 * 
 */
UCLASS()
class WARFORAPPLE_API UWFAGameData : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float SaveHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float SaveMaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float SaveAttack = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float SaveDefense = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float SaveCharisma = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float SaveRage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Inventory")
	TArray<FWFA_Inventory> SaveInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Inventory")
	TArray<FWFA_Inventory> SaveEquipment = {FWFA_Inventory(), FWFA_Inventory(), FWFA_Inventory()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FWFA_Quest> SaveQuests = {FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest(), FWFA_Quest()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<int> SaveQuestsStages = {0,0,0,0,0,5,0,5,0,5,0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<bool> SaveCompletedQuests = {false, false, false, false, false, false, false, false, false, false, false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<bool> SaveUnlockedInvocations = {false, false, false, false, false, false, false, false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 SavePlayerSkillPoints = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	FString SavePlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 SavePlayerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 SavePlayerXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 SavePlayerXPToNextLevel = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Info")
	int32 SaveAppleJuice = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Skills")
	TArray<ESkillState> SaveSkills = {ESkillState::None, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked, ESkillState::Locked};
};
