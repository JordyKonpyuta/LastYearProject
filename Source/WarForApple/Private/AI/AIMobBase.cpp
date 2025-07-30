// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIMobBase.h"

#include "AI/AICMobBase.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Library/WFALibrary.h"

// Sets default values
AAIMobBase::AAIMobBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->InitSphereRadius(DetectionRadius);
	DetectionSphere->SetCollisionProfileName("Trigger");
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetGenerateOverlapEvents(true);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAIMobBase::OnDetectionSphereOverlapBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAIMobBase::OnDetectionSphereOverlapEnd);

}

// Called when the game starts or when spawned
void AAIMobBase::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void AAIMobBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AAIMobBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AAIMobBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{

	if (RemoveHealth(DamageAmount) && !bIsDead)
	{
		bIsDead = true;
		UWFALibrary::GetRPGPlayerState()->AddPlayerXP(DropExperienceAmount);
		UWFALibrary::GetRPGPlayerState()->AddAppleJuice(DropJuice);
		PlayDeathAnimation();
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool AAIMobBase::RemoveHealth(float HealthToRemove)
{
	float NewHealth = Health - (HealthToRemove / Defense);
	PlayDamageAnimation();

	if (NewHealth <= 0)
	{
		Health = 0;
		return true;
	}
	SetHealth(NewHealth);
	return false;
}

void AAIMobBase::PlayDamageAnimation_Implementation()
{
}

void AAIMobBase::PlayDeathAnimation_Implementation()
{
	
}

void AAIMobBase::OnDetectionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor!= this)
	{
		if (AIControllerBase->IsValidLowLevelFast())
			AIControllerBase->OnPlayerDetected(OtherActor, true);
	}
}

void AAIMobBase::OnDetectionSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor!= this)
	{
		if (AIControllerBase->IsValidLowLevelFast())
		{
			AIControllerBase->OnPlayerDetected(OtherActor, false);
			
		}
	}
}
