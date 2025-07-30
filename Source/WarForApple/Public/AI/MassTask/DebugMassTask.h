// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMinimal.h"
#include "MassStateTreeTypes.h"
#include "StateTreeExecutionTypes.h"
#include "DebugMassTask.generated.h"

/**
 * 
 */
USTRUCT()
struct WARFORAPPLE_API FMassDebugTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bDebug = false;
};

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Mass Debug Task"))
struct WARFORAPPLE_API FMassDebugTaskInstance : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMassDebugTaskInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FMassDebugTaskInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};
