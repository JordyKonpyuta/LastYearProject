// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/WFAGameInstance.h"

#include "Kismet/GameplayStatics.h"


void UWFAGameInstance::Init()
{
	Super::Init();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		InitialiseAudioManager();
	}, 0.1f, false);

	LoadGame();
}

void UWFAGameInstance::Shutdown()
{
	Super::Shutdown();

	SaveGameIndex(UserIndex);
}

void UWFAGameInstance::ShutdownAfterError()
{
	Super::ShutdownAfterError();

	SaveGameIndex(UserIndex);
}

void UWFAGameInstance::SaveGame()
{
	if(GameData->IsValidLowLevel())
	{
		UGameplayStatics::SaveGameToSlot(GameData, SaveSlotName, UserIndex);
	}
}

void UWFAGameInstance::SaveGameIndex(int32 Index)
{
	if(GameData->IsValidLowLevel())
	{
		UGameplayStatics::SaveGameToSlot(GameData, SaveSlotName, Index);
	}
}

void UWFAGameInstance::LoadGame()
{
	// Vérifie si le fichier de sauvegarde existe
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// Charge le fichier de sauvegarde
		GameData = Cast<UWFAGameData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		// Crée un nouveau fichier de sauvegarde
		GameData = Cast<UWFAGameData>(UGameplayStatics::CreateSaveGameObject(UWFAGameData::StaticClass()));
	}
}

void UWFAGameInstance::LoadGameIndex(int32 Index)
{
	// Vérifie si le fichier de sauvegarde existe
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, Index))
	{
		// Charge le fichier de sauvegarde
		GameData = Cast<UWFAGameData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, Index));
	}
	else
	{
		// Crée un nouveau fichier de sauvegarde
		GameData = Cast<UWFAGameData>(UGameplayStatics::CreateSaveGameObject(UWFAGameData::StaticClass()));
	}
}

void UWFAGameInstance::ResetSaveGame()
{
	GameData = Cast<UWFAGameData>(UGameplayStatics::CreateSaveGameObject(UWFAGameData::StaticClass()));

	SaveGame();
}

void UWFAGameInstance::ResetSaveGameIndex(int32 Index)
{	
	GameData = Cast<UWFAGameData>(UGameplayStatics::CreateSaveGameObject(UWFAGameData::StaticClass()));

	SaveGameIndex(Index);
}

void UWFAGameInstance::InitialiseAudioManager()
{
	SoundManager = FAudioDevice::CreateComponent(MusicMainMenu, GetWorld()->GetAudioDeviceRaw());
	if(SoundManager->IsValidLowLevel())
	{
		SoundManager->SetVolumeMultiplier(1.f);
		SoundManager->SetPitchMultiplier(1.f);
		SoundManager->bAllowSpatialization = false;
		SoundManager->bIsUISound = true;
		SoundManager->bAutoDestroy = false;
		SoundManager->bIgnoreForFlushing = true;
		SoundManager->bStopWhenOwnerDestroyed = false;
	}
}

void UWFAGameInstance::PlaySoundInstance(USoundBase* Sound, float FadeOut)
{
	if(SoundManager->IsValidLowLevel())
	{
		if(Sound->IsValidLowLevel())
		{
			SoundManager->FadeOut(FadeOut, 0.f);
			// Wait for the fade out
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Sound, FadeOut]()
			{
				if(Sound->IsValidLowLevel())
				{
					SoundManager->Stop();
					SoundManager->SetSound(Sound);
					SoundManager->FadeIn(FadeOut, 1.f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Sound is not valid"));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sound is not valid"));
				}
			}, FadeOut + 1, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sound is not valid"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Sound is not valid"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundManager is not valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SoundManager is not valid"));
	}
}

void UWFAGameInstance::PlayMusicInGame(int32 Index, float FadeTime)
{
	if(MusicInGameList.IsValidIndex(Index))
	{
		PlaySoundInstance(MusicInGameList[Index], FadeTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MusicInGameList is not valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MusicInGameList is not valid"));
	}
}

void UWFAGameInstance::PlayNextMusicInGame(float FadeTime)
{
	if(SoundManager->IsValidLowLevel())
	{
		USoundBase* CurrentMusic = nullptr;
		USoundBase* NextMusic = nullptr;

		if(SoundManager->Sound->IsValidLowLevel())
		{
			CurrentMusic = SoundManager->Sound;
		}

		if(CurrentMusic->IsValidLowLevel())
		{
			// Check if the current music is in the list
			for(int32 i = 0; i < MusicInGameList.Num(); i++)
			{
				if(MusicInGameList[i] == CurrentMusic)
				{
					if(MusicInGameList.IsValidIndex(i + 1))
					{
						NextMusic = MusicInGameList[i + 1];
					}
					else
					{
						NextMusic = MusicInGameList[0];
					}
					break;
				}
			}

			if(!NextMusic->IsValidLowLevel())
			{
				if(MusicInGameList.IsValidIndex(0))
				{
					NextMusic = MusicInGameList[0];
				}
			}
		}
		else
		{
			if(MusicInGameList.IsValidIndex(0))
			{
				NextMusic = MusicInGameList[0];
			}
		}

		if(NextMusic)
		{
			PlaySoundInstance(NextMusic, FadeTime);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NextMusic is not valid"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NextMusic is not valid"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundManager is not valid"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SoundManager is not valid"));
	}
}
