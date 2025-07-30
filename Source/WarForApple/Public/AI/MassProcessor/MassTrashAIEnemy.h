// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassObserverProcessor.h"
#include "MassProcessor.h"
#include "AI/MassAIEnemy.h"
#include "AI/MassFragment/MassTrashAIEnemyFragment.h"
#include "MassTrashAIEnemy.generated.h"


class APlayers;
struct FMassMoveTargetFragment;
class AMassAIEnemy;
/**
 * 
 */
UCLASS()
class WARFORAPPLE_API UMassTrashAIEnemy : public UMassProcessor
{
	GENERATED_BODY()

	UMassTrashAIEnemy();

protected:

	bool RestMove = true;

	TObjectPtr<APlayers> PlayerRef = nullptr;
	
	float DelayTime = 4.f;
	
	float TimePassed = DelayTime;

	virtual void ConfigureQueries() override;

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	void Patrol(const FTransform& EntityTransform, FMassMoveTargetFragment& MoveTargetFragment, FMassExecutionContext& Context, EMassAIEnemyState
	            CurrentState, FMassEnemyFragment EnemyFragment);

	void Chase(const FVector& PlayerPosition ,	FMassMoveTargetFragment& MoveTargetFragment, const FVector& EntityLocation, FMassEnemyFragment EnemyFragment, FMassExecutionContext& Context);

	void Attack(FMassMoveTargetFragment& MoveTargetFragment, const FVector& PlayerPosition, const FVector& EntityLocation, FMassEnemyFragment
	            EnemyFragment, FMassExecutionContext& Context);

	void SelectActionMass(FMassMoveTargetFragment& MoveTargetFragment, const FVector& PlayerPosition,
	                  const FTransform& EntityTransform, FMassEnemyFragment AIEnemyFragment, FMassExecutionContext& Context, int32 EntityIdx);

	

private:

	FMassEntityQuery EntityQuery;
};

UCLASS()
class UMassTrashAIEnemyInitialization : public UMassProcessor
{
	GENERATED_BODY()

public:
	UMassTrashAIEnemyInitialization();

	bool bIsStarted = true;
	
	virtual void ConfigureQueries() override;

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	FMassEntityQuery EntityQuery;
	
};
