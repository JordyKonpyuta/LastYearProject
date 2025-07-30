// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MassProcessor/MassTrashAIEnemy.h"

#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassNavigationFragments.h"
#include "MassProcessor.h"
#include "MassRequirements.h"
#include "MassExecutionContext.h"
#include "MassActorSubsystem.h"
#include "MassEntityTypes.h"
#include "MassRepresentationTypes.h"
#include "AI/MassAIEnemy.h"
#include "AI/MassFragment/MassTrashAIEnemyFragment.h"
#include "Library/WFALibrary.h"


UMassTrashAIEnemy::UMassTrashAIEnemy()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;

	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);
}

void UMassTrashAIEnemy::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassMoveTargetFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadOnly);

	EntityQuery.AddRequirement<FMassEnemyFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);

	
	
}

void UMassTrashAIEnemy::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	
	if (!PlayerRef->IsValidLowLevelFast())
	{
		PlayerRef = UWFALibrary::GetRPGPlayer();
	}
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		auto AIEnemyFragments = Context.GetMutableFragmentView<FMassEnemyFragment>();
		auto TransformFragments = Context.GetFragmentView<FTransformFragment>();
		auto MoveTargetFragments = Context.GetMutableFragmentView<FMassMoveTargetFragment>();

		const int32 NumEntities = Context.GetNumEntities();

		for (int EntityIdx = 0; EntityIdx < NumEntities; EntityIdx++)
		{
			auto& MoveTargetFragment = MoveTargetFragments[EntityIdx];
			auto& AIEnemyFragment = AIEnemyFragments[EntityIdx];
			auto& TransformFragment = TransformFragments[EntityIdx];

			FTransform EntityTransform = TransformFragment.GetTransform();

			
			float DistanceToPlayer = 99999999999.f;


			if (AIEnemyFragment.AIEnemyRef)
			{
				if (AIEnemyFragment.AIEnemyRef->GetCurrentHealth() <= 0.f)
				{
					MoveTargetFragment.Center = MoveTargetFragment.Center = FVector::ZeroVector;
					MoveTargetFragment.Forward = FVector::ZeroVector;
					MoveTargetFragment.DesiredSpeed = FMassInt16Real(0.0f);
					MoveTargetFragment.CreateNewAction(EMassMovementAction::Stand, *Context.GetWorld());
					AIEnemyFragment.CurrentState = EMassAIEnemyState::Idle;
					if (AIEnemyFragment.AIEnemyRef->GetIsReadyToDie())
					{
						AIEnemyFragment.CurrentState = EMassAIEnemyState::Dead;
					}
					SelectActionMass( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform, AIEnemyFragment, Context, EntityIdx);
					continue;
				}
				
				if (AIEnemyFragment.AIEnemyRef->GetWasHit())
				{
					MoveTargetFragment.Center = MoveTargetFragment.Center = FVector::ZeroVector;
					MoveTargetFragment.Forward = FVector::ZeroVector;
					MoveTargetFragment.DesiredSpeed = FMassInt16Real(0.0f);
					MoveTargetFragment.CreateNewAction(EMassMovementAction::Stand, *Context.GetWorld());
					AIEnemyFragment.CurrentState = EMassAIEnemyState::Idle;
					SelectActionMass( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform, AIEnemyFragment, Context, EntityIdx);
					continue;
				}
				

				if (PlayerRef->IsValidLowLevelFast())
				{
					DistanceToPlayer = (PlayerRef->GetActorLocation() - EntityTransform.GetLocation()).Size();
				}
				
				
				if ( DistanceToPlayer > 2000.f && AIEnemyFragment.CurrentState != EMassAIEnemyState::Patrol )
				{
					AIEnemyFragment.CurrentState = EMassAIEnemyState::Patrol;
					MoveTargetFragment.Center = FVector::ZeroVector;
					SelectActionMass( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform, AIEnemyFragment, Context, EntityIdx);
					continue;
				}

				
				if (DistanceToPlayer < 1500.f && DistanceToPlayer > 251.0f && AIEnemyFragment.CurrentState != EMassAIEnemyState::Chase && AIEnemyFragment.CurrentState != EMassAIEnemyState::Attack )
				{
					AIEnemyFragment.CurrentState = EMassAIEnemyState::Chase;
					MoveTargetFragment.Center = FVector::ZeroVector;
					SelectActionMass( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform, AIEnemyFragment, Context, EntityIdx);
					continue;
				}

				if (DistanceToPlayer < 250.0f && AIEnemyFragment.CurrentState != EMassAIEnemyState::Attack )
				{
					MoveTargetFragment.CreateNewAction(EMassMovementAction::Stand, *Context.GetWorld());
					AIEnemyFragment.CurrentState = EMassAIEnemyState::Attack;
					SelectActionMass( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform, AIEnemyFragment, Context, EntityIdx);
				}
				SelectActionMass( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform, AIEnemyFragment, Context, EntityIdx);
			}
			
		}
	});
}

void UMassTrashAIEnemy::Patrol(const FTransform& EntityTransform ,	FMassMoveTargetFragment& MoveTargetFragment , FMassExecutionContext& Context,  EMassAIEnemyState CurrentState,
	FMassEnemyFragment EnemyFragment)
{
	FVector CurrentLocation = EntityTransform.GetLocation();
	FVector TargetLocation = MoveTargetFragment.Center - EntityTransform.GetLocation();
	TargetLocation.Z = EntityTransform.GetLocation().Z;
	MoveTargetFragment.DistanceToGoal = (TargetLocation).Size();
	MoveTargetFragment.Forward = TargetLocation.GetSafeNormal();

	float MaxDistanceForPatrol = EnemyFragment.MaxDistanceForPatrol;
	FVector PatrolCenterLocation = EnemyFragment.PatrolCenterLocation;
	

	if (MoveTargetFragment.DistanceToGoal <=  120.0f || MoveTargetFragment.Center == FVector::ZeroVector)
	{
		MoveTargetFragment.Center = FVector(FMath::RandRange(-MaxDistanceForPatrol, MaxDistanceForPatrol) + PatrolCenterLocation.X,
		FMath::RandRange(-MaxDistanceForPatrol, MaxDistanceForPatrol) + PatrolCenterLocation.Y, EnemyFragment.HeightBP);
		MoveTargetFragment.DistanceToGoal = (MoveTargetFragment.Center - CurrentLocation).Size();
		MoveTargetFragment.Forward = (MoveTargetFragment.Center - CurrentLocation).GetSafeNormal();
		MoveTargetFragment.DesiredSpeed = FMassInt16Real( 500.0f);
		MoveTargetFragment.CreateNewAction(EMassMovementAction::Move, *Context.GetWorld());
		MoveTargetFragment.IntentAtGoal = EMassMovementAction::Stand;
	}

}

void UMassTrashAIEnemy::Chase(const FVector& PlayerPosition, FMassMoveTargetFragment& MoveTargetFragment,
	const FVector& EntityLocation, FMassEnemyFragment EnemyFragment , FMassExecutionContext& Context)
{
	if ((MoveTargetFragment.Center - PlayerPosition).Size() >= 5.f)
	{
		FVector CurrentLocation = EntityLocation;
		FVector TargetLocation = MoveTargetFragment.Center - EntityLocation;
		TargetLocation.Z = EntityLocation.Z;
		MoveTargetFragment.DistanceToGoal = (TargetLocation).Size();
		MoveTargetFragment.Forward = TargetLocation.GetSafeNormal();

		MoveTargetFragment.Center = FVector( PlayerPosition.X, PlayerPosition.Y, EnemyFragment.HeightBP);
		MoveTargetFragment.DistanceToGoal = (MoveTargetFragment.Center - CurrentLocation).Size();
		MoveTargetFragment.Forward = (MoveTargetFragment.Center - CurrentLocation).GetSafeNormal();
		MoveTargetFragment.DesiredSpeed = FMassInt16Real( 800.0f);
		MoveTargetFragment.CreateNewAction(EMassMovementAction::Move, *Context.GetWorld());
		MoveTargetFragment.IntentAtGoal = EMassMovementAction::Stand;
	}
}

void UMassTrashAIEnemy::Attack(FMassMoveTargetFragment& MoveTargetFragment, const FVector& PlayerPosition, const FVector& EntityLocation,
	FMassEnemyFragment EnemyFragment , FMassExecutionContext& Context)
{
	TimePassed += GetWorld()->GetDeltaSeconds();
	
	if ((MoveTargetFragment.Center - PlayerPosition).Size() >= 5.f)
	{
		FVector CurrentLocation = EntityLocation;
		FVector TargetLocation = MoveTargetFragment.Center - EntityLocation;
		TargetLocation.Z = EntityLocation.Z;
		MoveTargetFragment.DistanceToGoal = (TargetLocation).Size();
		MoveTargetFragment.Forward = TargetLocation.GetSafeNormal();

		MoveTargetFragment.Center = FVector( PlayerPosition.X, PlayerPosition.Y, EnemyFragment.HeightBP);
		MoveTargetFragment.DistanceToGoal = (MoveTargetFragment.Center - CurrentLocation).Size();
		MoveTargetFragment.Forward = (MoveTargetFragment.Center - CurrentLocation).GetSafeNormal();
		MoveTargetFragment.DesiredSpeed = FMassInt16Real( 500.0f);
		MoveTargetFragment.CreateNewAction(EMassMovementAction::Move, *Context.GetWorld());
		MoveTargetFragment.IntentAtGoal = EMassMovementAction::Stand;
	}

	if ( (PlayerRef->GetActorLocation() - EntityLocation).Size() < 100.f && TimePassed >= DelayTime)
	{
		MoveTargetFragment.CreateNewAction(EMassMovementAction::Stand, *Context.GetWorld());
		EnemyFragment.AIEnemyRef->PlayAttackAnimation();
		TimePassed = 0.f;
	}
	
}
void UMassTrashAIEnemy::SelectActionMass(FMassMoveTargetFragment& MoveTargetFragment, const FVector& PlayerPosition,
	const FTransform& EntityTransform, FMassEnemyFragment AIEnemyFragment, FMassExecutionContext& Context,
	int32 EntityIdx)
{

	switch (AIEnemyFragment.CurrentState)
	{
	case EMassAIEnemyState::Idle:
		MoveTargetFragment.CreateNewAction(EMassMovementAction::Stand, *Context.GetWorld());
		break;
	case EMassAIEnemyState::Chase:
		Chase( PlayerRef->GetActorLocation(), MoveTargetFragment, EntityTransform.GetLocation(), AIEnemyFragment , Context);
		break;
	case EMassAIEnemyState::Attack:
		Attack( MoveTargetFragment, PlayerRef->GetActorLocation(), EntityTransform.GetLocation(), AIEnemyFragment , Context);
		break;
	case EMassAIEnemyState::Dead:
		Context.Defer().DestroyEntity(Context.GetEntity(EntityIdx));
		break;
	case EMassAIEnemyState::Patrol:
		Patrol( EntityTransform, MoveTargetFragment, Context, AIEnemyFragment.CurrentState, AIEnemyFragment);
		break;
	}
}

UMassTrashAIEnemyInitialization::UMassTrashAIEnemyInitialization()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;

	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Representation);
}

void UMassTrashAIEnemyInitialization::ConfigureQueries()
{
	EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassEnemyFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);
	
}

void UMassTrashAIEnemyInitialization::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	if (bIsStarted)
	{
		EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
		{
			auto ActorFragments = Context.GetFragmentView<FMassActorFragment>();
			auto EnemyFragments = Context.GetMutableFragmentView<FMassEnemyFragment>();
			auto TransformFragments = Context.GetMutableFragmentView<FTransformFragment>();
		
			const int32 NumEntities = Context.GetNumEntities();
			for (int EntityIdx = 0; EntityIdx < NumEntities; EntityIdx++)
			{
				TransformFragments[EntityIdx].SetTransform(FTransform(TransformFragments[EntityIdx].GetTransform().GetRotation(),
					FVector(TransformFragments[EntityIdx].GetTransform().GetLocation().X,TransformFragments[EntityIdx].GetTransform().GetLocation().Y,EnemyFragments[EntityIdx].HeightBP),
					TransformFragments[EntityIdx].GetTransform().GetScale3D()));
				
				if ( const AMassAIEnemy* AIEnemy = Cast<AMassAIEnemy>(ActorFragments[EntityIdx].Get()))
				{
					AMassAIEnemy* AIEnemyRef = const_cast<AMassAIEnemy*>(AIEnemy);
					EnemyFragments[EntityIdx].AIEnemyRef = AIEnemyRef;
				}
			}
		});
	}
}


