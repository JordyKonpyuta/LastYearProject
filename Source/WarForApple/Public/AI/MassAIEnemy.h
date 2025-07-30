// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassAIEnemy.generated.h"

class UMassAgentComponent;
class UCapsuleComponent;

UENUM(Blueprintable)
enum class EMassAIEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Dead UMETA(DisplayName = "Dead"),
	Patrol UMETA(DisplayName = "Patrol")
};

UCLASS()
class WARFORAPPLE_API AMassAIEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMassAIEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	

protected:

	UPROPERTY(Category = "AIStats", EditAnywhere)
	EMassAIEnemyState CurrentState = EMassAIEnemyState::Patrol;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	float CurrentHealth = 20;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	float MaxHealth = 20;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	float AttackDamage;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	float Defence = 10;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	int DropExperienceAmount = 1;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	int DropJuice = 1;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	bool bWasHit = false;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	bool bIsDead = false;

	UPROPERTY(Category = "AIStats", EditAnywhere)
	bool bIsReadyToDie = false;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ResetHit();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//GETTERS
	UFUNCTION(BlueprintCallable)
	EMassAIEnemyState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	float GetAttack() const { return AttackDamage; }

	UFUNCTION(BlueprintCallable)
	float GetDefence() const { return Defence; }

	UFUNCTION(BlueprintCallable)
	int GetDropExperienceAmount() const { return DropExperienceAmount; }

	UFUNCTION(BlueprintCallable)
	bool GetIsReadyToDie() const { return bIsReadyToDie; }
	
	UFUNCTION(BlueprintCallable)
	bool GetWasHit() const { return bWasHit; }

	//SETTERS

	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EMassAIEnemyState NewState);

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewHealth);

	UFUNCTION(BlueprintCallable)
	void SetAttackDamage(float NewDamage);

	UFUNCTION(BlueprintCallable)
	void SetDefence(float NewDefence);

	UFUNCTION(BlueprintCallable)
	void SetDropExperienceAmount(int NewExperienceAmount);

	UFUNCTION(BlueprintCallable)
	void SetIsReadyToDie(bool NewIsReadyToDie) { bIsReadyToDie = NewIsReadyToDie; }
	
	UFUNCTION(BlueprintCallable)
	void SetWasHit(bool bWasHitCheck) { bWasHit = bWasHitCheck; }
	
	bool RemoveHealth(float Health);

	//Animations
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayAttackAnimation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayDeathAnimation();
	
	
};




