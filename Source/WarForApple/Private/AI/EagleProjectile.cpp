// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EagleProjectile.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEagleProjectile::AEagleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName("Projectile");
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->OnComponentHit.AddDynamic(this, &AEagleProjectile::OnHit);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f; // No gravity

	//Projectile Homing
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = 1000.f;
	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	
}

// Called when the game starts or when spawned
void AEagleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEagleProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit!"));
	if( OtherActor == TargetActor)
	{
		// Apply damage to the target actor
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, nullptr);
		
		// Destroy the projectile after hitting the target
		Destroy();
	}
	else
	{
		// Destroy the projectile if it hits something else
		Destroy();
	}
}

// Called every frame
void AEagleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

