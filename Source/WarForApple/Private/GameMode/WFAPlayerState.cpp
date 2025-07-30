// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/WFAPlayerState.h"
#include "Character/Players.h"
#include "GameMode/WFAGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/WFALibrary.h"

void AWFAPlayerState::BeginPlay()
{
	Super::BeginPlay();
	// Get Game Instance
	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			// Set Player Data
			PlayerName = GameInstance->GetGameData()->SavePlayerName;
			PlayerLevel = GameInstance->GetGameData()->SavePlayerLevel;
			PlayerXP = GameInstance->GetGameData()->SavePlayerXP;
			PlayerXPToNextLevel = GameInstance->GetGameData()->SavePlayerXPToNextLevel;
			AppleJuice = GameInstance->GetGameData()->SaveAppleJuice;
			PlayerSkillPoints = GameInstance->GetGameData()->SavePlayerSkillPoints;

			// Inventory
			Inventory = GameInstance->GetGameData()->SaveInventory;
			Equipment = GameInstance->GetGameData()->SaveEquipment;

			// Quests
			Quests = GameInstance->GetGameData()->SaveQuests;
			QuestsStages = GameInstance->GetGameData()->SaveQuestsStages;
			CompletedQuests = GameInstance->GetGameData()->SaveCompletedQuests;
			UnlockedInvocations = GameInstance->GetGameData()->SaveUnlockedInvocations;
		}
	}
}


void AWFAPlayerState::SetPlayerUserName(FString NewPlayerName)
{
	PlayerName = NewPlayerName;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SavePlayerName = PlayerName;
		}
	}
}

void AWFAPlayerState::AddPlayerLevel(int32 PlayerLevelToAdd)
{
	SetPlayerLevel(GetPlayerLevel() + PlayerLevelToAdd);
}

void AWFAPlayerState::SetPlayerLevel(int32 NewPlayerLevel)
{	
	PlayerLevel = NewPlayerLevel;
	
	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SavePlayerLevel = PlayerLevel;
		}
	}
}

void AWFAPlayerState::AddPlayerXP(int32 PlayerXPToAdd)
{
	SetPlayerXP(GetPlayerXP() + PlayerXPToAdd);
}

void AWFAPlayerState::SetPlayerXP(int32 NewPlayerXP)
{	
	PlayerXP = NewPlayerXP;

	OnPlayerXPChange();
	CheckLevelUp();

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SavePlayerXP = PlayerXP;
		}
	}
}

void AWFAPlayerState::SetPlayerXPToNextLevel(int32 NewPlayerXPToNextLevel)
{	
	PlayerXPToNextLevel = NewPlayerXPToNextLevel;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SavePlayerXPToNextLevel = PlayerXPToNextLevel;
		}
	}
}

void AWFAPlayerState::AddAppleJuice(int32 NewAppleJuice)
{
	SetAppleJuice(GetAppleJuice() + NewAppleJuice);
}

void AWFAPlayerState::RemoveAppleJuice(int32 NewAppleJuice)
{
	SetAppleJuice(GetAppleJuice() - NewAppleJuice);
}

void AWFAPlayerState::SetAppleJuice(int32 NewAppleJuice)
{
	if(NewAppleJuice < 0)
	{
		AppleJuice = 0;
	}
	else
	{
		AppleJuice = NewAppleJuice;
	}

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveAppleJuice = AppleJuice;
		}
	}

	OnAppleJuiceChange();
}

void AWFAPlayerState::SetPlayerSkillPoints(int32 NewSkillPoints)
{
	if (NewSkillPoints < 0)
	{
		PlayerSkillPoints = 0;
	}
	else
	{
		PlayerSkillPoints = NewSkillPoints;
	}

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SavePlayerSkillPoints = PlayerSkillPoints;
		}
	}
}


// Player State Functions

void AWFAPlayerState::CheckLevelUp()
{
	if(PlayerXP >= PlayerXPToNextLevel)
	{
		int XPLeft = PlayerXP - PlayerXPToNextLevel;

		AddPlayerLevel(1);
		AddPlayerSkillPoints(1);
		
		if(XPLeft > 0)
			SetPlayerXP(XPLeft);
		else
			SetPlayerXP(0);
		SetPlayerXPToNextLevel(UKismetMathLibrary::FTrunc(FMath::Pow(GetPlayerLevel() + 1, 2.5f) + 1));

		// Stats
		UWFALibrary::GetRPGPlayer()->SetAttack(UWFALibrary::GetRPGPlayer()->GetAttack() + 3);
		UWFALibrary::GetRPGPlayer()->SetMaxHealth(UWFALibrary::GetRPGPlayer()->GetMaxHealth() + 5);
		UWFALibrary::GetRPGPlayer()->SetHealth(UWFALibrary::GetRPGPlayer()->GetMaxHealth());
		
		OnPlayerLevelUp();
		CheckLevelUp();
	}
}

void AWFAPlayerState::AddInventory(FWFA_Inventory ItemToAdd)
{
	Inventory.Add(ItemToAdd);

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveInventory = Inventory;
		}
	}
}

void AWFAPlayerState::RemoveInventory(int Index)
{
	Inventory.RemoveAt(Index);

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveInventory = Inventory;
		}
	}
}

void AWFAPlayerState::RemoveInventory(FWFA_Inventory ItemToRemove)
{
	//Inventory.Remove(ItemToRemove);
}

void AWFAPlayerState::ClearInventory()
{
	Inventory.Empty();

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveInventory = Inventory;
		}
	}
}

int AWFAPlayerState::SearchInInventory(FString nameToFind)
{
	int i = 0;
	for (FWFA_Inventory item : Inventory)
	{
		if (item.Name == nameToFind)
		{
			i++;
		}
	}

	return i;
}
