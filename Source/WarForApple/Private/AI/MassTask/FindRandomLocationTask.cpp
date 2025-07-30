// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MassTask/FindRandomLocationTask.h"


#include "MassCommonFragments.h"
#include "MassStateTreeTypes.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

bool FFindRandomLocationTask::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(TransformHandle);
	
	return FMassStateTreeTaskBase::Link(Linker);
}

EStateTreeRunStatus FFindRandomLocationTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("FFindRandomLocationTask::EnterState") );
	if (Transition.ChangeType != EStateTreeStateChangeType::Changed) { return EStateTreeRunStatus::Running; }
	TRACE_CPUPROFILER_EVENT_SCOPE(ST_FindRandomLocation)
	
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const float Range = InstanceData.Range;
	FTransformFragment& Transform = Context.GetExternalData(TransformHandle);
	const auto NewOffset = FVector(FMath::RandRange(Range/2*-1, Range/2), FMath::RandRange(Range/2*-1, Range/2), 0.f);;

	InstanceData.OutLocation = Transform.GetTransform().GetLocation()+NewOffset;
	
	return EStateTreeRunStatus::Running;
}
