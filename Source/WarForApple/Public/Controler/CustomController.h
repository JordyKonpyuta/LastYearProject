// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/WFA_Item.h"
#include "CustomController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class WARFORAPPLE_API ACustomController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FWFA_DPadShotcuts> DpadShortcuts = {FWFA_DPadShotcuts(), FWFA_DPadShotcuts(), FWFA_DPadShotcuts(), FWFA_DPadShotcuts()};
	
};
