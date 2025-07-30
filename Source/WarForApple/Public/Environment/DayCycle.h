// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "GameFramework/Actor.h"
#include "DayCycle.generated.h"

class ADirectionalLight;
class UDirectionalLightComponent;

UCLASS()
class WARFORAPPLE_API ADayCycle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayCycle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	USceneComponent* BaseRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	UDirectionalLightComponent* SunLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	UStaticMeshComponent* SkySphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	UMaterialInstanceDynamic* SkySphereMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	FRotator SunRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	float StartTime = 350.0f; // Start time of day in seconds

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	float DayLength = 600.0f; // Length of the day in seconds
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	float DaySpeed = 1.0f; // Speed of the day cycle
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	float CurrentTime; // Current time of day in seconds

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	bool bIsDayTime; // Is it currently daytime or nighttime

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Cycle")
	bool bIsNightTime; // Is it currently nighttime or daytime

	UPROPERTY()
	float NightLogic = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
