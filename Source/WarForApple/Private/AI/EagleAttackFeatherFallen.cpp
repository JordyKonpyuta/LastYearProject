// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EagleAttackFeatherFallen.h"

#include "NiagaraComponent.h"
#include "Character/Players.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEagleAttackFeatherFallen::AEagleAttackFeatherFallen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(250.f);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetAutoActivate(true);
	
}

// Called when the game starts or when spawned
void AEagleAttackFeatherFallen::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3);

	GetWorldTimerManager().SetTimer( DamageTimer, this, &AEagleAttackFeatherFallen::SendDamage, 01.f, false);
	
}

void AEagleAttackFeatherFallen::SendDamage()
{
	TArray<AActor*> OverlapActors;
	SphereComponent->GetOverlappingActors(OverlapActors);

	if (!OverlapActors.IsEmpty())
	{
		for (AActor* Actor : OverlapActors)
		{
			if (Cast<APlayers>(Actor))
			{
				UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, GetOwner(), nullptr);
			}
		}
	}

	
	
}

// Called every frame
void AEagleAttackFeatherFallen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

