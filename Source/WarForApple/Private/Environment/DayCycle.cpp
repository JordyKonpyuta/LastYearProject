// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/DayCycle.h"

#include "Components/DirectionalLightComponent.h"

// Sets default values
ADayCycle::ADayCycle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseRootComponent"));
	RootComponent = BaseRootComponent;

	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	SunLight->SetupAttachment(BaseRootComponent);
	SunLight->SetRelativeRotation(FRotator(-30.552490f,-180.000000f,180.000000f));
	SunLight->SetDynamicShadowDistanceMovableLight(10000.0f);

	SkySphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphereMesh"));
	SkySphereMesh->SetupAttachment(BaseRootComponent);
	SkySphereMesh->SetRelativeScale3D(FVector(400.0f, 400.0f, 400.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SkySphereMeshAsset(TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"));
	if (SkySphereMeshAsset.Succeeded())
	{
		SkySphereMesh->SetStaticMesh(SkySphereMeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> SkySphereMaterialAsset(TEXT("MaterialInstance'/Game/Assets/Materials/Sky/M_CustomSkyDome_Inst.M_CustomSkyDome_Inst'"));
	if (SkySphereMaterialAsset.Succeeded())
	{
		SkySphereMesh->SetMaterial(0, SkySphereMaterialAsset.Object);
	}
}

// Called when the game starts or when spawned
void ADayCycle::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the current time of day
	CurrentTime = StartTime;

	SkySphereMaterial = UMaterialInstanceDynamic::Create(SkySphereMesh->GetMaterial(0), this);
	if (SkySphereMaterial->IsValidLowLevel())
	{
		SkySphereMesh->SetMaterial(0, SkySphereMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance for SkySphere"));
	}
}

// Called every frame
void ADayCycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the current time of day
	CurrentTime += DeltaTime * DaySpeed;
	if (CurrentTime >= DayLength)
	{
		CurrentTime = 0.0f; // Reset the time to 0 when a full day has passed
	}
	// Calculate the sun's rotation based on the current time
	SunRotation.Pitch = FMath::Lerp(0.0f, 360.0f, CurrentTime / DayLength);
	SunRotation.Yaw = 0.0f;
	SunRotation.Roll = 0.0f;
	// Set the sun's rotation
	if (SunLight)
	{
		SunLight->SetWorldRotation(SunRotation);
	}

	// Determine if it's day or night
	bIsNightTime = CurrentTime < (DayLength / 2.0f);
	bIsDayTime = !bIsNightTime;
	
	// Update the sky sphere material parameters
	if (SkySphereMaterial->IsValidLowLevel())
	{
		NightLogic = FMath::FInterpTo(NightLogic, bIsNightTime ? 1.0f : 0.0f, DeltaTime, 1.0f);
		SkySphereMaterial->SetScalarParameterValue(TEXT("IsNight"), NightLogic);
	}
}

