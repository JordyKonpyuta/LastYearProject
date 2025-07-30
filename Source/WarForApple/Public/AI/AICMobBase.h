// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICMobBase.generated.h"

/**
 * 
 */
UENUM(Blueprintable)
enum class EAIState : uint8
{
	AS_Idle UMETA(DisplayName = "Idle"),
	AS_Move UMETA(DisplayName = "Move"),
	AS_Chase UMETA(DisplayName = "Chase"),
	AS_Attack UMETA(DisplayName = "Attack"),
	AS_Dead UMETA(DisplayName = "Dead"),
	AS_Patrol UMETA(DisplayName = "Patrol")
};


UCLASS()
class WARFORAPPLE_API AAICMobBase : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIState")
	EAIState AIStatus = EAIState::AS_Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AITarget")
	TObjectPtr<AActor> TargetActor = nullptr;
	

	virtual void OnPossess(APawn* InPawn) override;

//protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PlayerDetected")
	void OnPlayerDetected(AActor* Player, bool bIsInRange);

	UFUNCTION(BlueprintCallable)
	void SetAIStatus(EAIState NewStatus);
	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RestartStateTree();
	
};
