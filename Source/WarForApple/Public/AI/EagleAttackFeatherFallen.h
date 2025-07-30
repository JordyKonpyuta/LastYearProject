// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EagleAttackFeatherFallen.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class WARFORAPPLE_API AEagleAttackFeatherFallen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEagleAttackFeatherFallen();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack" , meta = (ExposeOnSpawn = true))
	float Damage = 10.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DamageTimer;

	void SendDamage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
