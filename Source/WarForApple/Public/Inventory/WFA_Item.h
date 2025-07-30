// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WFA_Item.generated.h"

// INVENTORY SYSTEM

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Claw"),
	Armour UMETA(DisplayName = "Fur"),
	Charisma UMETA(DisplayName = "Fang"),
	Consumable UMETA(DisplayName = "Consumable"),
	Invocation UMETA(DisplayName = "Invocation")
};

UENUM(BlueprintType)
enum class EConsumableType : uint8
{
	None UMETA(DisplayName = "None"),
	Berry UMETA(DisplayName = "Berry"),
	Dragonfly UMETA(DisplayName = "Dragonfly"),
	Truffle UMETA(DisplayName = "Truffle"),
	Pear UMETA(DisplayName = "Pear")
};

UENUM(BlueprintType)
enum class EStatGainType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Rage UMETA(DisplayName = "Rage"),
	Both UMETA(DisplayName = "Both")
};

UENUM(BlueprintType)
enum class EInvocationType : uint8
{
	None UMETA(DisplayName = "None"),
	Boar UMETA(DisplayName = "Boar"),
	Cat UMETA(DisplayName = "Cat"),
	Wasp UMETA(DisplayName = "Wasp"),
	Eagle UMETA(DisplayName = "Eagle"),
	Crocodile UMETA(DisplayName = "Crocodile"),
	Snake UMETA(DisplayName = "Snake"),
	Bear UMETA(DisplayName = "Bear"),
	Chicken UMETA(DisplayName = "Chicken")
};

USTRUCT(BlueprintType)
struct WARFORAPPLE_API FWFA_Inventory
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float StatGained;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EStatGainType StatGainType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* Icon;


	FWFA_Inventory()
	{
		ItemType = EItemType::Armour;
		Price = 0;
		StatGained = 0;
		Name = "";
		Description = "";
		Icon = nullptr;
		StatGainType = EStatGainType::Health;
	}

	FWFA_Inventory(EItemType itemType, float statGained, float price, FString name,
		FString description, UTexture2D* icon, int rank, EStatGainType statGainType)
	{
		ItemType = itemType;
		Price = price;
		StatGained = statGained;
		Name = name;
		Description = description;
		Icon = icon;
		StatGainType = statGainType;
	}
};

// D-PAD SHORTCUTS SYSTEM

USTRUCT(BlueprintType)
struct FWFA_DPadShotcuts
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DpadShotcuts")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DpadShotcuts")
	EInvocationType InvocationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DpadShotcuts")
	EConsumableType ConsumableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DpadShotcuts")
	FString ShortcutName;

	FWFA_DPadShotcuts()
	{
		ItemType = EItemType::Consumable;
		InvocationType = EInvocationType::None;
		ConsumableType = EConsumableType::None;
		ShortcutName = "";
	}

	FWFA_DPadShotcuts(EItemType itemType, EInvocationType invocationType, EConsumableType consumableType, FString name)
	{
		ItemType = itemType;
		InvocationType = invocationType;
		ConsumableType = consumableType;
		ShortcutName = name;
	}
};


// QUEST SYSTEM

USTRUCT(BlueprintType)
struct FWFA_QuestStages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestStages")
	FString StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestStages")
	FString StageDescription;

	FWFA_QuestStages()
	{
		StageName = "";
		StageDescription = "";
	}

	FWFA_QuestStages(FString stageName, FString stageDescription)
	{
		StageName = stageName;
		StageDescription = stageDescription;
	}
};

USTRUCT(BlueprintType)
struct FWFA_Quest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FWFA_Inventory Reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int RewardQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FWFA_QuestStages> QuestStages;

	FWFA_Quest()
	{
		QuestID = -9999;
		QuestStages = {FWFA_QuestStages()};
		QuestName = "";
		QuestDescription = "";
		Reward = FWFA_Inventory();
		RewardQuantity = 0;
	}

	FWFA_Quest(FString questName, FString questDescription, FWFA_Inventory reward, TArray<FWFA_QuestStages> questStages, int questID, int rewardQuantity)
	{
		QuestName = questName;
		QuestDescription = questDescription;
		Reward = reward;
		QuestID = questID;
		QuestStages = questStages;
		RewardQuantity = rewardQuantity;
	}
	
};

UENUM(BlueprintType)
enum class EOutpostType : uint8
{
	VerdantOrchad UMETA(DisplayName = "VerdantOrchad"),
	CobaltSpiritWoods UMETA(DisplayName = "CobaltSpiritWoods"),
	ScarletForest UMETA(DisplayName = "ScarletForest")
};

// DIALOGUE SYSTEM

USTRUCT(BlueprintType)
struct FWFA_Dialogue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FString> DialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool IsQuest;

	FWFA_Dialogue()
	{
		IsQuest = false;
		DialogueLines = {};
	}

	FWFA_Dialogue(TArray<FString> lines, bool isQuest)
	{
		DialogueLines = lines;
		IsQuest = isQuest;
	}
};

// SAVE SYSTEM

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	None UMETA(DisplayName = "None"),
	Locked UMETA(DisplayName = "Locked"),
	Get UMETA(DisplayName = "Get")
};

