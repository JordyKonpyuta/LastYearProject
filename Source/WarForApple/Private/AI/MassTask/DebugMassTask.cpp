// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MassTask/DebugMassTask.h"
#include "MassStateTreeExecutionContext.h"

bool FMassDebugTaskInstance::Link(FStateTreeLinker& Linker)
{
	return true;
}

EStateTreeRunStatus FMassDebugTaskInstance::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("FMassDebugTaskInstance::Print : ") + FString::FromInt(InstanceData.bDebug) );
	return FMassStateTreeTaskBase::EnterState(Context, Transition);
}

EStateTreeRunStatus FMassDebugTaskInstance::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	return FMassStateTreeTaskBase::Tick(Context, DeltaTime);
}

void FMassDebugTaskInstance::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FMassStateTreeTaskBase::ExitState(Context, Transition);
}
