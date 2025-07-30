// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "AudioDevice.h"
#include "Data/WFAGameData.h"
#include "WFAGameInstance.generated.h"

class USoundBase;
class UAudioComponent;
class FAudioDevice;
class UWFAGameData;

/**
 * 
 */
UCLASS()
class WARFORAPPLE_API UWFAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// DATA //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UWFAGameData* GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SaveSlotName = "WarForAppleSaveSlot";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 UserIndex = 0;

	// SOUND MANAGER //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundManager")
	UAudioComponent* SoundManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundManager")
	TArray<USoundBase*> MusicInGameList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundManager")
	USoundBase* MusicMainMenu;

public:

	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void ShutdownAfterError() override;

	// DATA //
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Data")
	UWFAGameData* GetGameData() const { return GameData; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Data")
	FString GetSaveSlotName() const { return SaveSlotName; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Data")
	int32 GetUserIndex() const { return UserIndex; }

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetGameData(UWFAGameData* NewGameData) { GameData = NewGameData; }

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetSaveSlotName(FString NewSaveSlotName) { SaveSlotName = NewSaveSlotName; }

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetUserIndex(int32 NewUserIndex) { UserIndex = NewUserIndex; }
	
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void SaveGameIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void LoadGameIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void ResetSaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void ResetSaveGameIndex(int32 Index);
	

	// SOUND MANAGER
	UFUNCTION(BlueprintCallable, Category = "SoundManager")
	void InitialiseAudioManager();

	UFUNCTION(BlueprintCallable, Category = "SoundManager")
	void PlaySoundInstance(USoundBase* Sound, float FadeOut);

	UFUNCTION(BlueprintCallable, Category = "SoundManager")
	void PlayMusicInGame(int32 Index, float FadeTime);

	UFUNCTION(BlueprintCallable, Category = "SoundManager")
	void PlayNextMusicInGame(float FadeTime);
};
