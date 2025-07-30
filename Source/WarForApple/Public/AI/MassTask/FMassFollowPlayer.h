// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassStateTreeTypes.h"
#include "FMassFollowPlayer.generated.h"

struct FMassMoveTargetFragment;
struct FTransformFragment;

USTRUCT()
struct WARFORAPPLE_API FMassFollowPlayerInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	AActor* TargetActor = nullptr;
};

/**
 * 
 */
USTRUCT( meta = (DisplayName = "WFAMassFollowPlayer"))
struct WARFORAPPLE_API FMassFollowPlayerData : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMassFollowPlayerInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FMassFollowPlayerInstanceData::StaticStruct(); }

	void FollowPlayer(FStateTreeExecutionContext& Context, EStateTreeRunStatus& StateTreeStatus) const;

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual void StateCompleted(FStateTreeExecutionContext& Context, const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const override;

protected:

	TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;
	TStateTreeExternalDataHandle<FMassMoveTargetFragment> MoveTargetHandle;
};

