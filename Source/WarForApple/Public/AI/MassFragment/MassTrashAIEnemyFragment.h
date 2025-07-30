// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassEntityTraitBase.h"
#include "AI/MassAIEnemy.h"

#include "MassTrashAIEnemyFragment.generated.h"


class AMassAIEnemy;

USTRUCT(BlueprintType)
struct FMassEnemyFragment : public FMassFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AMassAIEnemy> AIEnemyRef;

	EMassAIEnemyState CurrentState = EMassAIEnemyState::Patrol;

	UPROPERTY(EditAnywhere)
	float RaduisPatrol = 100.f;

	UPROPERTY(EditAnywhere)
	float AttackDistance = 100.f;

	UPROPERTY(EditAnywhere)
	float HeightBP = 0.f;

	FVector PatrolCenterLocation = FVector::ZeroVector;

	float MaxDistanceForPatrol = 3000.f;
	
};

USTRUCT()
struct WARFORAPPLE_API FEnemyTag : public FMassTag
{
	GENERATED_BODY()
	
};

UCLASS(meta = (DisplayName = "Mass Trash AI"))
class WARFORAPPLE_API UMassEnemyTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

public:

	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;

	UPROPERTY(EditAnywhere, Category = "Mass Trash AI")
	FMassEnemyFragment EnemyFragment;
	
};
