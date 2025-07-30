// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Interface_NPC.h"
#include "GameFramework/Character.h"
#include "Inventory/WFA_Item.h"
#include "NPC_Character.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class NPC_Type : uint8
{
	Quest UMETA(DisplayName = "Quest"),
	Normal UMETA(DisplayName = "Normal"),
	Shop UMETA(DisplayName = "Shop")
};

UCLASS()
class ANPC_Character : public ACharacter, public IInterface_NPC
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Character();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString NPC_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	UTexture2D* NPC_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	NPC_Type NPC_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TArray <FString> DialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FWFA_Quest> Quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int QuestIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsGeraud = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FWFA_Dialogue> AvailableDialogues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int ActiveDialogueIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FWFA_Dialogue> DefaultDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bCanRotate = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Interface
	virtual void OnInteract_Implementation() override;

	// UI
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC")
	void DisplayWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC")
	void SetDialogueLines();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC")
	void SetActiveDialogueIndex();

	// Quest
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "NPC")
	void FinishQuest();
};
