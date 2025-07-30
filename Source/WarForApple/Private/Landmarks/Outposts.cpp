// Fill out your copyright notice in the Description page of Project Settings.


#include "Landmarks/Outposts.h"

#include "MassCommonFragments.h"
#include "MassEntityConfigAsset.h"
#include "MassEntitySubsystem.h"
#include "MassSpawnerSubsystem.h"
#include "AI/AIMobBase.h"
#include "AI/MassFragment/MassTrashAIEnemyFragment.h"
#include "Library/WFALibrary.h"

// -----------------------
	// --    Constructor    --
	// -----------------------

AOutposts::AOutposts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootOutpost = CreateDefaultSubobject<USceneComponent>("Outposts");
	RootComponent = RootOutpost;

	OutpostMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("OutpostMesh");
	OutpostMeshComponent->SetupAttachment(RootOutpost);

	UStaticMesh* CylMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;

	if (!OutpostMeshComponent->GetStaticMesh())
		OutpostMeshComponent->SetStaticMesh(CylMesh);

	BattleRangeCylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>("BattleRangeCircleMesh");
	BattleRangeCylinderMesh->SetupAttachment(RootOutpost);
	BattleRangeCylinderMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	BattleRangeCylinderMesh->SetStaticMesh(CylMesh);
	BattleRangeCylinderMesh->SetWorldScale3D(FVector(OutpostRange / 50, OutpostRange / 50, 10));

	OutpostRangeMat = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Script/Engine.Material'/Game/Assets/Materials/Outpost/M_CircleOutpostRange.M_CircleOutpostRange'")).Object;

	if (!OutpostRangeMat)
	{
		UE_LOG(LogTemp, Error, TEXT("Outpost material is invalid"));
		return;
	}
	
	if (bConquered)
		BattleRangeCylinderMesh->SetMaterial(0, OutpostRangeMat);
	else
		BattleRangeCylinderMesh->SetMaterial(0, OutpostRangeMat);
}

	// -----------------------
	// --     Override      --
	// -----------------------

void AOutposts::BeginPlay()
{
	Super::BeginPlay();

	DynOutpostRangeMat = UMaterialInstanceDynamic::Create(OutpostRangeMat, this);
	BattleRangeCylinderMesh->SetMaterial(0, DynOutpostRangeMat);

	if (!bConquered)
		SpawnMobEntity();
	
	GetWorldTimerManager().SetTimer(
		CheckConquestTimer,
		this,
		&AOutposts::CheckConquest,
		1.0,
		true);
}

void AOutposts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGettingConquered)
	{
		if (!DynOutpostRangeMat->IsValidLowLevelFast())
		{
			bGettingConquered = false;
			return;
		}
		
		ConqueredMatValue += DeltaTime * 0.4;
		
		if (ConqueredMatValue >= 1)
		{
			ConqueredMatValue = 1;
			bGettingConquered = false;
		}
		
		DynOutpostRangeMat->SetScalarParameterValue("ConquerBlendValue", ConqueredMatValue);
		
		if (!bGettingConquered)
			ConqueredMatValue = 0;
	}
}
	
// ------------------------
// --   Taking Outpost   --
// ------------------------

void AOutposts::CheckConquest()
{
	if (SpawnedEntities.IsEmpty() && ArraySpawnedMob.IsEmpty())
		bConquered = true;

	auto& EntityManager = GetWorld()->GetSubsystem<UMassEntitySubsystem>()->GetMutableEntityManager();

	for(FMassEntityHandle CurHandle : SpawnedEntities)
	{
		if (CurHandle.IsValid())
		{
			if (!EntityManager.IsEntityValid(CurHandle))
				SpawnedEntities.Remove(CurHandle);
		}
		else
		{
			SpawnedEntities.Remove(CurHandle);
		}
	}

	for(AAIMobBase* CurMob : ArraySpawnedMob)
	{
		if (!CurMob->IsValidLowLevelFast())
			ArraySpawnedMob.Remove(CurMob);
	}

	if (bConquered)
		HasConqueredOutpost();
}

void AOutposts::HasConqueredOutpost()
{
	GetWorldTimerManager().ClearTimer(CheckConquestTimer);
	
	if (DynOutpostRangeMat->IsValidLowLevelFast())
		DynOutpostRangeMat->SetScalarParameterValue("ConquerBlendValue", 1);

	UpdateMinimapIcon();
	
	switch (OutpostType)
	{
	case EOutpostType::VerdantOrchad:
		if (UWFALibrary::GetRPGPlayerState()->CompletedQuests[4] != true)
		{
			UWFALibrary::GetRPGPlayerState()->QuestsStages[4] = UWFALibrary::GetRPGPlayerState()->QuestsStages[4] + 1;
		}
		else
		{
			UWFALibrary::GetRPGPlayerState()->QuestsStages[5] = UWFALibrary::GetRPGPlayerState()->QuestsStages[5] + 1;
		}
		break;
	case EOutpostType::ScarletForest:
		if (UWFALibrary::GetRPGPlayerState()->CompletedQuests[6] != true)
		{
			UWFALibrary::GetRPGPlayerState()->QuestsStages[6] = UWFALibrary::GetRPGPlayerState()->QuestsStages[6] + 1;
		}
		else
		{
			UWFALibrary::GetRPGPlayerState()->QuestsStages[7] = UWFALibrary::GetRPGPlayerState()->QuestsStages[7] + 1;
		}
		break;
	case EOutpostType::CobaltSpiritWoods:
		if (UWFALibrary::GetRPGPlayerState()->CompletedQuests[8] != true)
		{
			UWFALibrary::GetRPGPlayerState()->QuestsStages[8] = UWFALibrary::GetRPGPlayerState()->QuestsStages[8] + 1;
		}
		else
		{
			UWFALibrary::GetRPGPlayerState()->QuestsStages[9] = UWFALibrary::GetRPGPlayerState()->QuestsStages[9] + 1;
		}
		break;
	}
}

	// -----------------------
	// --    Refresh Post   --
	// -----------------------

void AOutposts::RefreshOutpost()
{
	if (OutpostMesh->IsValidLowLevelFast())
		OutpostMeshComponent->SetStaticMesh(OutpostMesh);

	if (DynOutpostRangeMat->IsValidLowLevelFast())
	{
		if (bConquered)
		{
			DynOutpostRangeMat->SetScalarParameterValue("ConquerBlendValue", 1);
		}
		else
			DynOutpostRangeMat->SetScalarParameterValue("ConquerBlendValue", 0);
	}
	
	BattleRangeCylinderMesh->SetWorldScale3D(FVector(OutpostRange / 50, OutpostRange / 50, 10));
}
		
	// -----------------------
	// --    Enemy Spawn    --
	// -----------------------

FVector AOutposts::GetSpawnPoint()
{
	// CALCULATE X AND Y

	float RandX = FMath::RandRange(-OutpostRange, OutpostRange);
	const float MaxY = FMath::Sqrt((OutpostRange * OutpostRange) - (RandX * RandX));
	
	float RandY = FMath::RandRange(-MaxY, MaxY) + GetActorLocation().Y;
	RandX +=  GetActorLocation().X;

	// REMOVE FROM THE CENTER IF NEED BE
	if (	GetActorLocation().X + 100 > RandX && RandX > GetActorLocation().X - 100
		&&	GetActorLocation().Y + 100 > RandY && RandY > GetActorLocation().Y - 100)
	{
		if (FMath::RandBool())
		{
			if (RandX - GetActorLocation().X <= 0)
				RandX += FMath::RandRange(-OutpostRange, -100.f);
			else
				RandX += FMath::RandRange(100.f, OutpostRange);
		}
		else
		{
			if (RandY - GetActorLocation().Y <= 0)
				RandY += FMath::RandRange(-OutpostRange, -100.f);
			else
				RandY += FMath::RandRange(100.f, OutpostRange);
		}
	}

	// CALCULATE Z
	
	FCollisionQueryParams LineTraceParams = FCollisionQueryParams(FName(TEXT("Gravity Line Trace")), true, this);
	LineTraceParams.bTraceComplex = true;
	LineTraceParams.bReturnPhysicalMaterial = false;

	// INIT HIT INFO
	FHitResult LineHit(ForceInit);
	
	// LINE TRACE
	GetWorld()->LineTraceSingleByChannel(
		LineHit,
		FVector(RandX, RandY, 0),
		FVector(RandX, RandY, 1000),
		ECC_Pawn,
		LineTraceParams
	);
	
	return FVector(RandX, RandY, LineHit.ImpactPoint.Z);
}


void AOutposts::SpawnMassEntity(int NumberEntitySpawn)
{

	auto& SpawnerSystem = *GetWorld()->GetSubsystem<UMassSpawnerSubsystem>();
	auto& EntityManager = GetWorld()->GetSubsystem<UMassEntitySubsystem>()->GetMutableEntityManager();
	
	if (SpawnerSystem.IsValidLowLevel())
	{
		if (EntityConfig->IsValidLowLevelFast())
		{		
			SpawnerSystem.SpawnEntities(EntityConfig->GetOrCreateEntityTemplate(*GetWorld()), NumberEntitySpawn, SpawnedEntities);

			TArray<FVector> SpawnPoints;
			for (int32 i = 0; i < SpawnedEntities.Num(); i++)
			{
				auto& TransformFragment = EntityManager.GetFragmentDataChecked<FTransformFragment>(SpawnedEntities[i]);
				auto& EnemyFragment = EntityManager.GetFragmentDataChecked<FMassEnemyFragment>(SpawnedEntities[i]);

				TransformFragment.GetMutableTransform().SetLocation(GetSpawnPoint());

				EnemyFragment.PatrolCenterLocation = GetActorLocation();
				EnemyFragment.MaxDistanceForPatrol = OutpostRange * 0.8f;
			
			}
		}
	}
}

void AOutposts::SpawnMobEntity_Implementation()
{
}
