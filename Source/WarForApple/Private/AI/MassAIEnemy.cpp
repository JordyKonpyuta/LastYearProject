// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MassAIEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Library/WFALibrary.h"

// Sets default values
AMassAIEnemy::AMassAIEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCapsuleHalfHeight(34);
	CapsuleComponent->SetCapsuleRadius(34);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	SkeletalMeshComponent->SetRelativeLocation(FVector(0, 0, -34));
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
}

float AMassAIEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	bWasHit = true;
	
	if (RemoveHealth(DamageAmount) && !bIsDead)
	{
		bIsDead = true;
		UWFALibrary::GetRPGPlayerState()->AddPlayerXP(DropExperienceAmount);
		UWFALibrary::GetRPGPlayerState()->AddAppleJuice(DropJuice);
		PlayDeathAnimation();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer( TimerHandle, this, &AMassAIEnemy::ResetHit, 1.f, false);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called when the game starts or when spawned
void AMassAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMassAIEnemy::ResetHit()
{
	SetWasHit(false);
}


// Called every frame
void AMassAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMassAIEnemy::SetCurrentState(EMassAIEnemyState NewState)
{
	CurrentState = NewState;
}

void AMassAIEnemy::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void AMassAIEnemy::SetMaxHealth(float NewHealth)
{
	MaxHealth = NewHealth;
}

void AMassAIEnemy::SetAttackDamage(float NewDamage)
{
	AttackDamage = NewDamage;
}

void AMassAIEnemy::SetDefence(float NewDefence)
{
	Defence = NewDefence;
}

void AMassAIEnemy::SetDropExperienceAmount(int NewExperienceAmount)
{
	DropExperienceAmount = NewExperienceAmount;
}

bool AMassAIEnemy::RemoveHealth(float Health)
{
	float NewHealth = GetCurrentHealth() - (Health / GetDefence());
	
	if (NewHealth<= 0)
	{
		SetCurrentHealth(0);
		return true;
	}
	SetCurrentHealth(NewHealth);
	return false;
}

void AMassAIEnemy::PlayAttackAnimation_Implementation()
{
}

void AMassAIEnemy::PlayDeathAnimation_Implementation()
{
}

