// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Players.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UAnimMontage;
class UCapsuleComponent;
class UEnhancedInputComponent;
class UCharacterMovementComponent;
class UKismetMathLibrary;
class UGameplayStatics;
class APlayerStart;
class UMaterialInstance;
class ANPC_Character;

UCLASS()
class WARFORAPPLE_API APlayers : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayers();

	// PLAYER STATS //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float Attack = 1.0f; // Multiplicative

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float Defense = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float Charisma = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float Rage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float DetectFocusRadius = 1024.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float DistanceRadiusLost = 1600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Stats")
	float DodgeCooldown = 3.0f;

	// PLAYER COMPONENTS //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Component")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Component")
	UCameraComponent* Camera;

	// ENEMIES HIT //

	UPROPERTY()
	TArray<FHitResult> AllThingsHit;
	
	// PLAYER STATUS //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bGuarding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bFocusing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bDodging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bInteracting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Status")
	bool bIsInvincible;

	// PLAYER CAN //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Can")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Can")
	bool bCanLook = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Can")
	bool bCanQuickOptions = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Can")
	bool bCanInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Can")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Can")
	bool bCanDodge = true;

	// PLAYER MOVEMENT //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Movement")
	float RunSpeed = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Movement")
	float GuardSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Movement")
	float DodgeForce = 500.0f;

	// PLAYER LOCAL //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Movement")
	float UpDownMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Movement")
	float LeftRightMovement;

	// INPUT ACTION //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuickOptionsAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FocusAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	// ANIMATIONS LIST //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	UAnimMontage* HitMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	TArray<UAnimMontage*> HeavyAttackMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	UAnimMontage* PummelMontage;

	// ANIM NOTIFY VALUES //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	int CurNotifyCombo = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Animations")
	bool bLightAttack = true;

	// DEBRIS //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	AActor* targetActorFocus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	int ComboCounter = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	int ComboMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	UMaterialInstance* InvincibleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	UMaterialInstance* HitMaterial;

	UPROPERTY()
	FTimerHandle DodgeTimerHandle;

	UPROPERTY()
	FTimerHandle AttackTimerHandle;

	UPROPERTY()
	FTimerHandle InvincibleTimerHandle;

	// PUMMEL ATTACK VALUES //
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	bool bIsPummeling;

	FTimerHandle DetachTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Debris")
	AActor* CurrentPummelTarget;

	// NPC INTERACTION //

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Interaction")
	ANPC_Character* NPC_Ref = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player - Interaction")
	bool bIsEndDialogue = false;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Virtual take damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// PLAYER STATS - GETTERS //
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Stats")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Stats")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Stats")
	float GetAttack() const { return Attack; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Stats")
	float GetDefense() const { return Defense; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Stats")
	float GetCharisma() const { return Charisma; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Stats")
	float GetRage() const { return Rage; }
	
	
	// PLAYER STATUS - GETTERS //
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsAttacking() const { return bAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsGuarding() const { return bGuarding; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsFocusing() const { return bFocusing; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsDodging() const { return bDodging; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsInteracting() const { return bInteracting; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsDead() const { return bDead; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Status")
	bool IsInvincible() const { return bIsInvincible; }

	
	// PLAYER CAN - GETTERS //
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Can")
	bool CanMove() const { return bCanMove; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Can")
	bool CanLook() const { return bCanLook; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Can")
	bool CanQuickOptions() const { return bCanQuickOptions; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Can")
	bool CanInteract() const { return bCanInteract; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Can")
	bool CanAttack() const { return bCanAttack; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player - Can")
	bool CanDodge() const { return bCanDodge; }


	// PLAYER STATS - SETTERS //
	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void AddHealth(float HealthToAdd);

	UFUNCTION(Blueprintable, Category = "Player - Stats")
	void RemoveHealth(float HealthToRemove);

	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void SetAttack(float NewAttack);

	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void SetDefense(float NewDefense);

	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void SetCharisma(float NewCharisma);

	UFUNCTION(BlueprintCallable, Category = "Player - Stats")
	void SetRage(float NewRage);
	
	// PLAYER STATUS - SETTERS //
	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetRunning(bool bNewRunning);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetAttacking(bool bNewAttacking);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetGuarding(bool bNewGuarding);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetFocusing(bool bNewFocusing);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetDodging(bool bNewDodging);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetInteracting(bool bNewInteracting);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetDead(bool bNewDead);

	UFUNCTION(BlueprintCallable, Category = "Player - Status")
	void SetInvincible(bool bNewInvincible);

	// PLAYER CAN - SETTERS //
	UFUNCTION(BlueprintCallable, Category = "Player - Can")
	void SetCanMove(bool bNewCanMove);

	UFUNCTION(BlueprintCallable, Category = "Player - Can")
	void SetCanLook(bool bNewCanLook);

	UFUNCTION(BlueprintCallable, Category = "Player - Can")
	void SetCanQuickOptions(bool bNewCanQuickOptions);

	UFUNCTION(BlueprintCallable, Category = "Player - Can")
	void SetCanInteract(bool bNewCanInteract);

	UFUNCTION(BlueprintCallable, Category = "Player - Can")
	void SetCanAttack(bool bNewCanAttack);

	UFUNCTION(BlueprintCallable, Category = "Player - Can")
	void SetCanDodge(bool bNewCanDodge);

	// INPUT FUNCTIONS //
	
	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LookEnd(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void QuickOptions(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void Interact(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void LightAttack(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void HeavyAttack(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void StartGuard(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void EndGuard(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void Focus(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void Dodge(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void RunStart(const FInputActionInstance& Value);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Player - Input")
	void RunEnd(const FInputActionValue& Value);


	// GLOBAL FUNCTIONS //
	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void ResetDodging();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void AttackCombo(bool bIsLightAttack);

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void ResetCombotCounter();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void CheckDeath();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void Respawn();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void CallInvincibleLogic(float Time);
	
	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void StartInvincible();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void StopInvincible();
	
	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void StartSecretInvincible();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void StopSecretInvincible();

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void CheckIfCanAttach(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void AttachToEnemy(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Player - Global")
	void DetachFromEnemy();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Player - Global")
	void MoveCameraInDialogue();
};
