// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassStateTreeTypes.h"
#include "FindRandomLocationTask.generated.h"

class UMassSignalSubsystem;
struct FTransformFragment;

USTRUCT()
struct FFindRandomLocationTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Input)
	float Range = 1000.f;

	UPROPERTY(EditAnywhere, Category=Output)
	FVector OutLocation = FVector::ZeroVector;

	FFindRandomLocationTaskInstanceData() = default;
};

/**
 * Find a random location to wander to
 */
USTRUCT(meta = (DisplayName = "WFAFindRandomLocation"))
struct FFindRandomLocationTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FFindRandomLocationTaskInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FFindRandomLocationTaskInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
protected:
	TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;
};
