// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIMobBase.generated.h"

class AAICMobBase;
class USphereComponent;

UCLASS()
class WARFORAPPLE_API AAIMobBase : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIDetection")
	TObjectPtr<USphereComponent> DetectionSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIDetection")
	float DetectionRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIController")
	TObjectPtr<AAICMobBase> AIControllerBase = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIAnimation")
	TObjectPtr<UAnimMontage> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIAnimation")
	TObjectPtr<UAnimMontage> DeathMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIAnimation")
	TObjectPtr<UAnimMontage> DamageMontages;


	
protected:

	//AIStat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	float Health = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	float Attack = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	float Defense = 5.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	int DropExperienceAmount = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	int DropJuice = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "AIStat")
	bool bIsDead = false;

	
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDetectionSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnDetectionSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	// Sets default values for this character's properties
	AAIMobBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIStat")
	float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIStat")
	float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIStat")
	float GetAttack() const { return Attack; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AIStat")
	float GetDefense() const { return Defense; }

	//SETTERS
	UFUNCTION(BlueprintCallable, Category = "AIStat")
	void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = "AIStat")
	void SetHealth(float NewHealth) { Health = NewHealth; }
	UFUNCTION(BlueprintCallable, Category = "AIStat")
	void SetAttack(float NewAttack) { Attack = NewAttack; }
	UFUNCTION(BlueprintCallable, Category = "AIStat")
	void SetDefense(float NewDefense) { Defense = NewDefense;  }

	UFUNCTION(BlueprintCallable, Category = "AIStat")
	bool RemoveHealth(float HealthToRemove);


	//Anim
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AIAnimation")
	void PlayDeathAnimation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AIAnimation")
	void PlayDamageAnimation();
	


};
