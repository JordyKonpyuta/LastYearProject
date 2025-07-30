// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICMobBase.h"

#include "AI/AIMobBase.h"
#include "Character/Players.h"

void AAICMobBase::OnPossess(APawn* InPawn)
{
	Cast<AAIMobBase>(InPawn)->AIControllerBase = this;
	Super::OnPossess(InPawn);
}

void AAICMobBase::SetAIStatus(EAIState NewStatus)
{
	AIStatus = NewStatus;
	RestartStateTree();
}

void AAICMobBase::RestartStateTree_Implementation()
{
}

void AAICMobBase::OnPlayerDetected_Implementation(AActor* Player, bool bIsInRange)
{
	
	if (Cast<APlayers>(Player) && bIsInRange)
	{
		AIStatus= EAIState::AS_Chase;
		TargetActor = Player;
	}
	else if (!TargetActor->IsValidLowLevel() && !bIsInRange || TargetActor == Player)
	{
		AIStatus= EAIState::AS_Patrol;
	}
}
