// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/WFALibrary.h"

void UWFALibrary::TeleportPlayerAtActor(AActor* Player, AActor* TargetActor)
{
	if(Player->IsValidLowLevel())
	{
		if(TargetActor->IsValidLowLevel())
		{
			Player->SetActorLocation(TargetActor->GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[WFA] TargetActor is not valid for teleport"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("[WFA] TargetActor is not valid for teleport"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[WFA] Player is not valid for teleport"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("[WFA] Player is not valid for teleport"));
	}
}

void UWFALibrary::TeleportPlayerAtLocation(AActor* Player, FVector Location)
{
	if(Player->IsValidLowLevel())
	{
		Player->SetActorLocation(Location);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[WFA] Player is not valid for teleport"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("[WFA] Player is not valid for teleport"));
	}
}

APlayers* UWFALibrary::GetRPGPlayer()
{
	return Cast<APlayers>(UGameplayStatics::GetPlayerCharacter(GEngine->GameViewport->GetWorld(), 0));
}

ACustomController* UWFALibrary::GetRPGController()
{
	return Cast<ACustomController>(UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0));
}

AWFAPlayerState* UWFALibrary::GetRPGPlayerState()
{
	return Cast<AWFAPlayerState>(UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0)->PlayerState);
}

AWFAGameState* UWFALibrary::GetRPGGameState()
{
	return Cast<AWFAGameState>(UGameplayStatics::GetGameState(GEngine->GameViewport->GetWorld()));
}

AWFAGameMode* UWFALibrary::GetRPGGameMode()
{
	return Cast<AWFAGameMode>(UGameplayStatics::GetGameMode(GEngine->GameViewport->GetWorld()));
}

UWFAGameInstance* UWFALibrary::GetRPGGameInstance()
{	
	return Cast<UWFAGameInstance>(GEngine->GameViewport->GetWorld()->GetGameInstance());
}

bool UWFALibrary::DoesSaveGameExist()
{
	FString SaveSlotName = "WarForAppleSaveSlot";

	for(uint8 i = 0; i < UINT8_MAX ; i++)
	{
		if(UGameplayStatics::DoesSaveGameExist(SaveSlotName, i))
		{
			return true;
		}
	}
	
	return false;
}
