// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Players.h"

#include "KismetTraceUtils.h"
#include "AI/AICMobBase.h"
#include "AI/AIMobBase.h"
#include "AI/NPC/NPC_Character.h"
#include "Library/WFALibrary.h"

// Sets default values
APlayers::APlayers()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct Components
	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->CameraRotationLagSpeed = 20.0f;
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, FName("SpringEndpoint"));

	// Character Movement Settings
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->SetFixedBrakingDistance(200.0f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void APlayers::BeginPlay()
{
	Super::BeginPlay();

	ComboMax = LightAttackMontages.Num() - 1;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			Health = GameInstance->GetGameData()->SaveHealth;
			
			if(Health > MaxHealth)
			{
				Health = MaxHealth;
			}
			else if(Health <= 0.0f)
			{
				Health = MaxHealth;
			}
			
			MaxHealth = GameInstance->GetGameData()->SaveMaxHealth;
			Attack = GameInstance->GetGameData()->SaveAttack;
			Defense = GameInstance->GetGameData()->SaveDefense;
			Charisma = GameInstance->GetGameData()->SaveCharisma;
			Rage = GameInstance->GetGameData()->SaveRage;
		}
	}
}

// Called every frame
void APlayers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 500.0f, FColor::Red, false, 0.0f, 0, 5.0f);

	if(!IsDead())
	{
		if(GetCharacterMovement()->IsValidLowLevel())
		{
			GetCharacterMovement()->Velocity.Size() > 850.0f ? SetRunning(true) : SetRunning(false);
		}

		if(targetActorFocus->IsValidLowLevel() && IsFocusing())
		{
			// Recentrer la camera vers le targetActorFocus
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetActorFocus->GetActorLocation());

			// Centre Unique la camera sur l'axe Yaw
			//NewRotation.Pitch = 0.0f;
			//NewRotation.Roll = 0.0f;

			
			GetController()->SetControlRotation(NewRotation);

			// Verifier si le targetActorFocus est toujours dans le radius de detection
			if(FVector::Distance(targetActorFocus->GetActorLocation(), GetActorLocation()) > DistanceRadiusLost)
			{
				targetActorFocus = nullptr;
				SetFocusing(false);
			}
		}
		else if (!targetActorFocus->IsValidLowLevel() && IsFocusing())
		{
			targetActorFocus = nullptr;
			SetFocusing(false);
		}
	}
}

float APlayers::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if(!IsDead())
	{
		if(!IsInvincible() && !IsGuarding())
		{
			ComboCounter = -1;
			PlayAnimMontage(HitMontage);
			RemoveHealth(DamageAmount / GetDefense());
			CallInvincibleLogic(1.5f);
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called to bind functionality to input
void APlayers::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move & Look
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,this, &APlayers::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled,this, &APlayers::MoveEnd);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed,this, &APlayers::MoveEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,this, &APlayers::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Canceled,this, &APlayers::LookEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed,this, &APlayers::LookEnd);

		// Quick Options
		EnhancedInputComponent->BindAction(QuickOptionsAction, ETriggerEvent::Completed,this, &APlayers::QuickOptions);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed,this, &APlayers::Interact);

		// Combat
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Completed,this, &APlayers::LightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Completed,this, &APlayers::HeavyAttack);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Started,this, &APlayers::StartGuard);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Canceled,this, &APlayers::EndGuard);
		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed,this, &APlayers::EndGuard);
		EnhancedInputComponent->BindAction(FocusAction, ETriggerEvent::Started,this, &APlayers::Focus);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started,this, &APlayers::Dodge);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered,this, &APlayers::RunStart);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Canceled,this, &APlayers::RunEnd);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed,this, &APlayers::RunEnd);

	}
}

// INPUT FUNCTIONS //

void APlayers::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		if(CanMove() && !IsDead())
		{
			UpDownMovement = MovementVector.Y;
			LeftRightMovement = MovementVector.X;
			
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);

			// Auto Running
			if(!IsDodging() && !IsGuarding())
			{
				if(GetVelocity().Size() > 500.0f)
				{
					GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, RunSpeed, 0.02f);
				}
				else
				{	
					GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(GetCharacterMovement()->MaxWalkSpeed, WalkSpeed, 0.02f);
				}
			}
		}
	}
}

void APlayers::MoveEnd(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		if(CanMove() && !IsDead())
		{
			UpDownMovement = 0.0f;
			LeftRightMovement = 0.0f;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void APlayers::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		if(CanLook() && !IsDead())
		{
			AddControllerYawInput(LookVector.X);
			AddControllerPitchInput(LookVector.Y);
		}
	}
}

void APlayers::LookEnd(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		if(CanLook() && !IsDead())
		{
			
		}
	}
}

void APlayers::QuickOptions_Implementation(const FInputActionValue& Value)
{
}

void APlayers::Interact_Implementation(const FInputActionValue& Value)
{
	if (bCanInteract && !IsDead())
	{
		if (NPC_Ref != nullptr)
			NPC_Ref->OnInteract_Implementation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayers::Interact_Implementation"));
	}
}

void APlayers::LightAttack_Implementation(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		if(!IsDead() && !IsDodging() && !IsGuarding() && CanAttack())
		{
			AttackCombo(true);
		}
	}
}

void APlayers::HeavyAttack_Implementation(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		if(!IsDead() && !IsDodging() && !IsGuarding() && CanAttack())
		{
			AttackCombo(false);
		}
	}
}

void APlayers::StartGuard_Implementation(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		if(!IsDead() && !IsDodging())
		{
			SetGuarding(true);
			ComboCounter = -1;

			GetCharacterMovement()->MaxWalkSpeed = GuardSpeed;

			// Recentrer la camera vers le forward de l'actor
			FRotator NewRotation = GetActorRotation();
			NewRotation.Pitch = 0.0f;
			NewRotation.Roll = 0.0f;
			GetController()->SetControlRotation(NewRotation);

			bUseControllerRotationYaw = true;
		}
	}
}

void APlayers::EndGuard_Implementation(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		if(!IsDead() && !IsDodging())
		{
			SetGuarding(false);
			
			if(!IsRunning()) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			
			bUseControllerRotationYaw = false;
		}
	}
}

void APlayers::Focus_Implementation(const FInputActionValue& Value)
{
	bool bFocus = Value.Get<bool>();
	
	if(Controller != nullptr)
	{
		if(!IsGuarding() && !IsDodging() && !IsDead())
		{
			ComboCounter = -1;
			
			if(targetActorFocus->IsValidLowLevel() && IsFocusing())
			{
				targetActorFocus = nullptr;
				SetFocusing(false);
			}
			else
			{
				targetActorFocus = nullptr;

				FVector Start = GetActorLocation();
				FVector End = Start + GetActorForwardVector();
				TArray<FHitResult> HitResults;
				FCollisionQueryParams CollisionQueryParams;
				CollisionQueryParams.AddIgnoredActor(this);
				
				if(GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(DetectFocusRadius), CollisionQueryParams))
				{
					if(HitResults.Num() > 0)
					{
						TArray<AActor*> ActorsHasTag;
						for(FHitResult HitResult : HitResults)
						{
							if(HitResult.GetActor()->ActorHasTag("FocusableEnemy"))
							{
								ActorsHasTag.Add(HitResult.GetActor());
							}
						}

						if(ActorsHasTag.Num() > 0)
						{
							float MinDistance = 0.0f;
							AActor* ClosestActor = nullptr;
							
							for(AActor* Actor : ActorsHasTag)
							{
								float Distance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
								if(MinDistance == 0.0f || Distance < MinDistance)
								{
									MinDistance = Distance;
									ClosestActor = Actor;
								}
							}

							// Set le targetActorFocus
							targetActorFocus = ClosestActor;
						}
					}
				}

				if(targetActorFocus->IsValidLowLevel())
				{
					SetFocusing(true);
				}
			}
		}
	}
}

void APlayers::Dodge_Implementation(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		if(CanMove() && CanDodge() && !IsDead() && !IsDodging() && !IsGuarding())
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
			GetCharacterMovement()->GroundFriction = 0.0f;
			SetCanMove(false);
			SetCanDodge(false);
			SetDodging(true);
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
			ComboCounter = -1;
			CallInvincibleLogic(1.5f);
			
			FVector ForwardVector = GetActorForwardVector();
			ForwardVector.Normalize();

			// Multiplier la force de l'esquive
			ForwardVector *= DodgeForce;
			
			GetCharacterMovement()->AddImpulse(ForwardVector, true);

			PlayAnimMontage(DodgeMontage);

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([this]()
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
				GetCharacterMovement()->GroundFriction = 8.0f;
				SetCanMove(true);
				SetDodging(false);
				GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &APlayers::ResetDodging, DodgeCooldown, false);
			});
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
		}
	}
}

void APlayers::RunStart_Implementation(const FInputActionInstance& Value)
{
	if(Controller != nullptr)
	{
		if(!IsDead() && !IsGuarding())
		{
			if(GetVelocity() != FVector::ZeroVector) GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
	}
}

void APlayers::RunEnd_Implementation(const FInputActionValue& Value)
{
	if(Controller != nullptr)
	{
		
	}
}

// PLAYER STATS - GETTERS //

void APlayers::SetHealth(float NewHealth)
{

	if(NewHealth > MaxHealth)
	{
		Health = MaxHealth;
	}
	else if(NewHealth < 0.0f)
	{
		Health = 0.0f;
	}
	else
	{
		Health = NewHealth;
	}

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveHealth = Health;
		}
	}
	
	CheckDeath();
}

void APlayers::AddHealth(float HealthToAdd)
{
	SetHealth(GetHealth() + HealthToAdd);
}

void APlayers::RemoveHealth(float HealthToRemove)
{
	SetHealth(GetHealth() - HealthToRemove);
}

void APlayers::SetMaxHealth(float NewMaxHealth)
{	
	MaxHealth = NewMaxHealth;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveMaxHealth = MaxHealth;
		}
	}
}

void APlayers::SetAttack(float NewAttack)
{	
	Attack = NewAttack;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveAttack = Attack;
		}
	}
}

void APlayers::SetDefense(float NewDefense)
{	
	Defense = NewDefense;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveDefense = Defense;
		}
	}
}

void APlayers::SetCharisma(float NewCharisma)
{	
	Charisma = NewCharisma;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveCharisma = Charisma;
		}
	}
}

void APlayers::SetRage(float NewRage)
{	
	Rage = NewRage;

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		if(GameInstance->GetGameData()->IsValidLowLevel())
		{
			GameInstance->GetGameData()->SaveRage = Rage;
		}
	}
}

// PLAYER STATUS - SETTERS //
void APlayers::SetRunning(bool bNewRunning)
{	
	bIsRunning = bNewRunning;
}

void APlayers::SetAttacking(bool bNewAttacking)
{	
	bAttacking = bNewAttacking;
}

void APlayers::SetGuarding(bool bNewGuarding)
{	
	bGuarding = bNewGuarding;
}

void APlayers::SetFocusing(bool bNewFocusing)
{	
	bFocusing = bNewFocusing;
}

void APlayers::SetDodging(bool bNewDodging)
{	
	bDodging = bNewDodging;
}

void APlayers::SetInteracting(bool bNewInteracting)
{	
	bInteracting = bNewInteracting;
}

void APlayers::SetDead(bool bNewDead)
{	
	bDead = bNewDead;
}

void APlayers::SetInvincible(bool bNewInvincible)
{
	bIsInvincible = bNewInvincible;
}

// PLAYER CAN - SETTERS //

void APlayers::SetCanMove(bool bNewCanMove)
{	
	bCanMove = bNewCanMove;
}

void APlayers::SetCanLook(bool bNewCanLook)
{	
	bCanLook = bNewCanLook;
}

void APlayers::SetCanQuickOptions(bool bNewCanQuickOptions)
{	
	bCanQuickOptions = bNewCanQuickOptions;
}

void APlayers::SetCanInteract(bool bNewCanInteract)
{	
	bCanInteract = bNewCanInteract;
}

void APlayers::SetCanAttack(bool bNewCanAttack)
{	
	bCanAttack = bNewCanAttack;
}

void APlayers::SetCanDodge(bool bNewCanDodge)
{	
	bCanDodge = bNewCanDodge;
}

// GLOBAL FUNCTION //

void APlayers::ResetDodging()
{
	SetCanDodge(true);
}

// Systeme de Combo d'Attaque
void APlayers::AttackCombo(bool bIsLightAttack)
{
	if (bIsPummeling)
	{
		if (!bIsLightAttack)
			return;
		
		PlayAnimMontage(PummelMontage);
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		ComboCounter = -1;
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APlayers::ResetCombotCounter, 0.8f, false);
		
		return;
	}
	
	if(ComboCounter < ComboMax)
	{
		ComboCounter++;
	}
	else
	{
		ComboCounter = 0;
	}
	
	CurNotifyCombo = ComboCounter;

	if(IsFocusing() && targetActorFocus->IsValidLowLevel())
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetActorFocus->GetActorLocation());
		NewRotation = FRotator(0.0f, NewRotation.Yaw, 0.0f);
		SetActorRotation(NewRotation);
	}

	if(bIsLightAttack)
	{
		bLightAttack = true;
		PlayAnimMontage(LightAttackMontages[ComboCounter]);
	}
	else
	{
		bLightAttack = false;
		PlayAnimMontage(HeavyAttackMontages[ComboCounter]);
	}

	if(!bIsLightAttack)
	{
		ComboCounter = -1;
	}

	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	if(bIsLightAttack) GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APlayers::ResetCombotCounter, 2.0f, false);
}

void APlayers::ResetCombotCounter()
{	
	ComboCounter = -1;
}

void APlayers::CheckDeath()
{
	if(GetHealth() <= 0.0f)
	{
		SetDead(true);
		SetCanMove(false);
		SetCanLook(false);
		SetCanQuickOptions(false);
		SetCanInteract(false);
		SetCanAttack(false);
		SetCanDodge(false);

		GetCharacterMovement()->StopActiveMovement();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		UWFALibrary::GetRPGPlayerState()->PlayRespawnAnimation();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayers::Respawn, 5.0f, false);
	}
}

void APlayers::Respawn()
{
	SetHealth(GetMaxHealth());
	SetDead(false);
	SetCanMove(true);
	SetCanLook(true);
	SetCanQuickOptions(true);
	SetCanInteract(true);
	SetCanAttack(true);
	SetCanDodge(true);
	SetInvincible(false);
	SetGuarding(false);
	
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetMesh()->SetRelativeLocation( FVector(0.0f, 0.0f, -90.0f) );
	GetMesh()->SetRelativeRotation( FRotator(0.0f, -90.0f, 0.0f) );
	
	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), "RespawnLevel", FoundActors);

	if(FoundActors.Num() > 0)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActors[FMath::RandRange(0, FoundActors.Num() - 1)]);
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}

	if(UWFAGameInstance* GameInstance = UWFALibrary::GetRPGGameInstance())
	{
		GameInstance->SaveGame();
	}
}

void APlayers::CallInvincibleLogic(float Time)
{	
	StartInvincible();
	GetWorld()->GetTimerManager().ClearTimer(InvincibleTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(InvincibleTimerHandle, this, &APlayers::StopInvincible, Time, false);
}

void APlayers::StartInvincible()
{
	SetInvincible(true);

	if(InvincibleMaterial->IsValidLowLevel())
	{
		if(GetMesh()->IsValidLowLevel())
		{
			GetMesh()->SetOverlayMaterial(InvincibleMaterial);
		}
	}
}

void APlayers::StopInvincible()
{
	SetInvincible(false);

	if(GetMesh()->IsValidLowLevel())
	{
		GetMesh()->SetOverlayMaterial(nullptr);
	}
}

void APlayers::StartSecretInvincible()
{
	SetInvincible(true);
}

void APlayers::StopSecretInvincible()
{
	SetInvincible(false);
}

void APlayers::CheckIfCanAttach(AActor* Target)
{
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation(), 250.f, TraceTypeQuery2, true,
		TArray<AActor*>{this}, EDrawDebugTrace::ForDuration,AllThingsHit,true,
		FColor::Red,FColor::Green, 2.0f);

	if (!AllThingsHit.IsEmpty())
	{
		for (FHitResult CurHit : AllThingsHit)
		{
			if (CurHit.GetActor()->IsValidLowLevelFast())
			{
				if (CurHit.GetActor() == Target && Target->Tags.Contains("BigEnemy"))
				{
					AttachToEnemy(CurHit.GetActor());
				}
			}
		}
	}
}

void APlayers::AttachToEnemy(AActor* Target)
{
	CurrentPummelTarget = Target;
	const FAttachmentTransformRules PummelAttachRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	AttachToActor(CurrentPummelTarget, PummelAttachRules);
	
	if (AAICMobBase* TargetController = Cast<AAIMobBase>(CurrentPummelTarget)->AIControllerBase)
		TargetController->SetAIStatus(EAIState::AS_Idle);
	
	GetWorldTimerManager().SetTimer(
		DetachTimerHandle,
		this,
		&APlayers::DetachFromEnemy,
		3.0f,
		false);

	bIsPummeling = true;
}

void APlayers::DetachFromEnemy()
{
	const FDetachmentTransformRules PummelDetachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
	DetachFromActor(PummelDetachRules);
	
	if (AAICMobBase* TargetController = Cast<AAIMobBase>(CurrentPummelTarget)->AIControllerBase)
		TargetController->SetAIStatus(EAIState::AS_Chase);
	
	CurrentPummelTarget = nullptr;
	bIsPummeling = false;
}
