// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntitySubsystem.h"
#include "GameFramework/Actor.h"
#include "Inventory/WFA_Item.h"
#include "Outposts.generated.h"

struct FMassEntityHandle;
class UMassEntityConfigAsset;
class AAIMobBase;

UCLASS()
class WARFORAPPLE_API AOutposts : public AActor
{
	GENERATED_BODY()

	// UPROPERTIES
public:	
	// -----------------------
	// --       Meshes      --
	// -----------------------

	UPROPERTY()
	TObjectPtr<USceneComponent> RootOutpost;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> OutpostMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> OutpostMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BattleRangeCylinderMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> BattleRangeMesh;

	// -----------------------
	// --     Materials     --
	// -----------------------

	UPROPERTY()
	TObjectPtr<UMaterialInterface> OutpostRangeMat;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynOutpostRangeMat;

	UPROPERTY()
	bool bGettingConquered = false;

	UPROPERTY()
	float ConqueredMatValue = 0;

	// ------------------------
	// --       Values       --
	// ------------------------
	
	UPROPERTY(EditAnywhere)
	float OutpostRange = 5000;

	UPROPERTY(EditAnywhere)
	bool bConquered = false;

	UPROPERTY(EditAnywhere)
	bool bIsInPeriphery = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OutpostIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EOutpostType OutpostType;
	
	// ------------------------
	// --        Boss        --
	// ------------------------

	UPROPERTY(EditAnywhere)
	TMap<TObjectPtr<AAIMobBase>, int32> BossToSpawn;

	// -----------------------
	// --    SpawnEntity    --
	// -----------------------
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMassEntityConfigAsset> EntityConfig;

	TArray<FMassEntityHandle> SpawnedEntities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AAIMobBase>> ArraySpawnedMob;
	
protected:
	FTimerHandle CheckConquestTimer;

	// UFUNCTIONS
public:
	// -----------------------
	// --    Constructor    --
	// -----------------------
	
	AOutposts();
	
	// -----------------------
	// --     Override      --
	// -----------------------
	
	virtual void Tick(float DeltaTime) override;
	
	// ------------------------
	// --   Taking Outpost   --
	// ------------------------

	UFUNCTION(BlueprintCallable)
	void CheckConquest();

	UFUNCTION(BlueprintCallable)
	void HasConqueredOutpost();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateMinimapIcon();

protected:
	// -----------------------
	// --     Override      --
	// -----------------------
	
	virtual void BeginPlay() override;
	
	// -----------------------
	// --    Refresh Post   --
	// -----------------------

	UFUNCTION(Category="Refresh", CallInEditor)
	void RefreshOutpost();
	
	// -----------------------
	// --    Enemy Spawn    --
	// -----------------------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintCallable)
	void SpawnMassEntity(int NumberEntitySpawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnMobEntity();

	// -----------------------
	// --    InfoOutpost    --
	// -----------------------

	//returns true if the outpost is conquered
	UFUNCTION(BlueprintCallable)
	bool OutpostsIsConquered()
	{
		return bConquered;
	}

};
