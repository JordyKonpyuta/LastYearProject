// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MassTask/FMassFollowPlayer.h"

#include "MassCommonFragments.h"
#include "MassNavigationFragments.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"


bool FMassFollowPlayerData::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(TransformHandle);
	Linker.LinkExternalData(MoveTargetHandle);
	
	
	return FMassStateTreeTaskBase::Link(Linker);
}

void FMassFollowPlayerData::FollowPlayer(FStateTreeExecutionContext& Context,
	EStateTreeRunStatus& StateTreeStatus) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	FMassMoveTargetFragment MoveTarget = Context.GetExternalData(MoveTargetHandle);
	const FTransformFragment TransformFragment = Context.GetExternalData(TransformHandle);

	AActor* TargetActor = InstanceData.TargetActor;

	if ((TargetActor->GetActorLocation() - TransformFragment.GetTransform().GetLocation()).Length() <= 100)
	{
		StateTreeStatus = EStateTreeRunStatus::Succeeded;
	}
	else
	{
		MoveTarget.Center = TargetActor->GetActorLocation();
		MoveTarget.SlackRadius = 50.f;
		MoveTarget.Forward = (MoveTarget.Center - TransformFragment.GetTransform().GetLocation()).GetSafeNormal();
		MoveTarget.DistanceToGoal = FVector::Dist(MoveTarget.Center, TransformFragment.GetTransform().GetLocation());
		MoveTarget.CreateNewAction(EMassMovementAction::Move, *Context.GetWorld());
		MoveTarget.IntentAtGoal = EMassMovementAction::Stand;
	}
	
}

EStateTreeRunStatus FMassFollowPlayerData::EnterState(FStateTreeExecutionContext& Context,
                                                      const FStateTreeTransitionResult& Transition) const
{
	return FMassStateTreeTaskBase::EnterState(Context, Transition);
}

EStateTreeRunStatus FMassFollowPlayerData::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	return FMassStateTreeTaskBase::Tick(Context, DeltaTime);
}

void FMassFollowPlayerData::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FMassStateTreeTaskBase::ExitState(Context, Transition);
}

void FMassFollowPlayerData::StateCompleted(FStateTreeExecutionContext& Context,
	const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates) const
{
	FMassStateTreeTaskBase::StateCompleted(Context, CompletionStatus, CompletedActiveStates);
}
