// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EagleProjectile.generated.h"

class UArrowComponent;

UCLASS()
class WARFORAPPLE_API AEagleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEagleProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileMovement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;


	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Damage" , meta = ( ExposeOnSpawn = true))
	float Damage = 10.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Target" , meta = ( ExposeOnSpawn = true))
	TObjectPtr<AActor> TargetActor = nullptr;


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
