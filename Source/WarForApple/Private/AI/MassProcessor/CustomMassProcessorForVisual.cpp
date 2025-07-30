// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MassProcessor/CustomMassProcessorForVisual.h"
#include "MassCommonFragments.h"
#include "MassRepresentationFragments.h"
#include "MassEntitySubsystem.h"
#include "AI/MassAIEnemy.h"
#include "AI/MassFragment/MassActorFragmentVisual.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

UCustomMassProcessorForVisual::UCustomMassProcessorForVisual()
{
	// Activer l'enregistrement automatique avec les phases de traitement
	//bAutoRegisterWithProcessingPhases = true;

	
	// Définir l'ordre d'exécution (le plus tôt possible dans le pipeline de rendu)
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Representation;
}

void UCustomMassProcessorForVisual::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassActorFragmentVisual>(EMassFragmentAccess::ReadWrite);
}

void UCustomMassProcessorForVisual::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& QueryContext)
{
	const int32 NumEntities = QueryContext.GetNumEntities();
        
	// Récupération des Fragments
	TArrayView<FMassActorFragmentVisual> ActorFragments = QueryContext.GetMutableFragmentView<FMassActorFragmentVisual>();
	TConstArrayView<FTransformFragment> Transforms = QueryContext.GetFragmentView<FTransformFragment>();

	for (int32 i = 0; i < NumEntities; i++)
	{
		// Récupère la transformation de l'entité
		const FTransform& EntityTransform = Transforms[i].GetTransform();

		

		// Vérifie si un acteur est déjà assigné
		if (!ActorFragments[i].SpawnedActor.IsValid())
		{
			// Crée un acteur Blueprint pour le rendu
			AActor* SpawnedActor = Context.GetWorld()->SpawnActor<AActor>(AMassAIEnemy::StaticClass(), EntityTransform);
                
			if (SpawnedActor)
			{
				// Associe l'acteur à l'entité
				ActorFragments[i].SpawnedActor = SpawnedActor;
			}
		}
	}
});
}
