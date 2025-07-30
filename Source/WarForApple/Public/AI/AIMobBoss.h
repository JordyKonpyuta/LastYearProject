// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIMobBase.h"
#include "AIMobBoss.generated.h"

class APlayers;
/**
 * 
 */
UCLASS()
class WARFORAPPLE_API AAIMobBoss : public AAIMobBase
{
	GENERATED_BODY()

public:
		
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	TObjectPtr<UArrowComponent> ArrowProjectile;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> Attack2Montages;
	

	AAIMobBoss();


	
};
