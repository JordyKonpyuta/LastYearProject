// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"

void UCustomGameInstance::RunWfa(FString Directory)
{
	FPlatformProcess::CreateProc(*Directory.Append(TEXT("\\Build_WFA\\WarForApple.exe")), nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
}
