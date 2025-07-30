#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassCommonFragments.h"
#include "MassEntityView.h"
#include "UObject/ObjectMacros.h"
#include "MassActorFragmentVisual.generated.h"

USTRUCT(BlueprintType)
struct FMassActorFragmentVisual : public FMassFragment
{
	GENERATED_BODY()

	// Stocke une référence à l’Actor utilisé pour l’affichage
	UPROPERTY()
	TWeakObjectPtr<AActor> SpawnedActor;
};