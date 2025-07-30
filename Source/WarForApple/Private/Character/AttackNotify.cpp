// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AttackNotify.h"

#include "Character/Players.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TObjectPtr<APlayers> Player = Cast<APlayers>(MeshComp->GetAttachParentActor());
	TArray<FHitResult> AllThingsHit;

	if (!Player->IsValidLowLevelFast())
		return;

	
	float EndVectorSpeedAdditive = UKismetMathLibrary::Square(Player->GetVelocity().X) + UKismetMathLibrary::Square(Player->GetVelocity().Y);
	EndVectorSpeedAdditive = UKismetMathLibrary::Sqrt(EndVectorSpeedAdditive);

	// ATTACK PROPERTIES
	bool bKb = false;
	bool bAttract = false;
	bool bAttach = false;

	ETraceTypeQuery TraceTypeCustom = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	
	float HitMultiplier = 1.f;

	if (Player->bIsPummeling)
		UGameplayStatics::ApplyDamage(Player->CurrentPummelTarget, Player->GetAttack() * 0.5f, Player->GetController(), Player, nullptr);
	else if (Player->bLightAttack)
		switch (Player->CurNotifyCombo)
		{
		case 0:
			HitMultiplier = 1.f;
			UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + FVector(0.f,0.f,-150.f) ,
				Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(250.0f, 250.0f, 450.0f) * (1 + EndVectorSpeedAdditive / 1800),
				FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
				TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
				AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			break;
		case 1:
			HitMultiplier = 1.f;
			UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * 75.f,
				Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(200.0f, 200.0f, 250.0f) * (1 + EndVectorSpeedAdditive / 900),
				FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
				TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
				AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			break;
		case 2:
			HitMultiplier = 1.2;
			UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() - Player->GetActorForwardVector() * 75.f ,
				Player->GetActorLocation() + Player->GetActorForwardVector() * 100.f * (1 + EndVectorSpeedAdditive / 900),
				FVector(90.0f, 90.0f, 100.0f), Player->GetActorRotation(),
				TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
				AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			break;
		case 3:
			if (CurNotifyState == 0)
			{
				HitMultiplier = 0.f;
				FVector Direction = Player->GetActorForwardVector();
				Direction.Z = 0.25;
				Direction = Direction * 1350.f;
				
				Player->GetCharacterMovement()->StopActiveMovement();
				Player->GetCharacterMovement()->Launch(Direction);
			}
			else if (CurNotifyState == 1)
			{
				HitMultiplier = 1.f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() - Player->GetActorForwardVector() * 75.f  ,
					Player->GetActorLocation() + Player->GetActorForwardVector() * 100.f * (1 + EndVectorSpeedAdditive / 900),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			break;
		case 4:
			HitMultiplier = 1.2f;
			if (CurNotifyState == 0)
			{
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * 150.f,
					Player->GetActorLocation() + Player->GetActorRightVector() * 150.f,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
            			
				bKb = true;
			}
			else if (CurNotifyState == 1)
			{
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * 150.f,
					Player->GetActorLocation() + Player->GetActorForwardVector() * -150.f,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
            			
				bKb = true;
			}
			else if (CurNotifyState == 2)
			{
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * -150.f,
					Player->GetActorLocation() + Player->GetActorRightVector() * -150.f,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
            			
				bKb = true;
			}
			else if (CurNotifyState == 3)
			{
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * -150.f,
					Player->GetActorLocation() + Player->GetActorForwardVector() * 150.f,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
				
				bKb = true;
			}
			else if (CurNotifyState == 4)
			{
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * 150.f,
					Player->GetActorLocation(),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
				
				bKb = true;
			}
			break;
		default:
			break;
		}
	else
		switch (Player->CurNotifyCombo)
		{
		case 0:
			HitMultiplier = 0.5f;
			UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Player->GetActorLocation(),
				Player->GetActorLocation(), 350.f, TraceTypeCustom, true,
				TArray<AActor*>{Player}, EDrawDebugTrace::None,AllThingsHit,true,
				FColor::Red,FColor::Green, 2.0f);
			break;
		case 1:
			UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation(),
				Player->GetActorLocation() + Player->GetActorForwardVector() * 800,
				FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
				TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
				AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			bAttach = true;
			break;
		case 2:
			if (CurNotifyState == 0)
			{
				HitMultiplier = 0.7f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * 75.f,
					Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(200.0f, 200.0f, 250.0f) * (1 + EndVectorSpeedAdditive / 900),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 1)
			{
				HitMultiplier = 0.7f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * -75.f,
					Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(200.0f, 200.0f, 250.0f) * (1 + EndVectorSpeedAdditive / 900),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 2)
			{
				HitMultiplier = 0.7f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * 75.f,
					Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(200.0f, 200.0f, 250.0f) * (1 + EndVectorSpeedAdditive / 900),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 3)
			{
				HitMultiplier = 0.7f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * -75.f,
					Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(200.0f, 200.0f, 250.0f) * (1 + EndVectorSpeedAdditive / 900),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);		
			}
			else if (CurNotifyState == 4)
			{
				HitMultiplier = 1.2f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + FVector(0.f,0.f,-150.f) ,
					Player->GetActorLocation() + Player->GetActorForwardVector() * FVector(250.0f, 250.0f, 250.0f) * (1 + EndVectorSpeedAdditive / 1800),
					FVector(60.0f, 80.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
				
				bKb = true;
			}
			break;
		case 3:
			if (CurNotifyState == 0)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation(),
					Player->GetActorLocation() + Player->GetActorForwardVector() * 250,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 1)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * 250,
					Player->GetActorLocation() + Player->GetActorForwardVector() * 250 + Player->GetActorRightVector() * -200,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 2)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * 250 + Player->GetActorRightVector() * -200,
					Player->GetActorLocation() + Player->GetActorRightVector() * -250,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 3)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * -250,
					Player->GetActorLocation() + Player->GetActorForwardVector() * -250 + Player->GetActorRightVector() * -200,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);		
			}
			else if (CurNotifyState == 4)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * -250 + Player->GetActorRightVector() * -200,
					Player->GetActorLocation() + Player->GetActorForwardVector() * -250 + Player->GetActorRightVector() * 200,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);	
			}
			else if (CurNotifyState == 5)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * -250 + Player->GetActorRightVector() * 200,
					Player->GetActorLocation() + Player->GetActorRightVector() * 250,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 6)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorRightVector() * 250,
					Player->GetActorLocation() + Player->GetActorForwardVector() * 100 + Player->GetActorRightVector() * 200,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 7)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * 100 + Player->GetActorRightVector() * 200,
					Player->GetActorLocation() + Player->GetActorForwardVector() * 200 + Player->GetActorRightVector() * 150,
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);
			}
			else if (CurNotifyState == 8)
			{
				HitMultiplier = 0.8f;
				UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Player->GetActorLocation() + Player->GetActorForwardVector() * 200 + Player->GetActorRightVector() * 150,
					Player->GetActorLocation(),
					FVector(70.0f, 70.0f, 100.0f), Player->GetActorRotation(),
					TraceTypeCustom, true, TArray<AActor*>{Player}, EDrawDebugTrace::None,
					AllThingsHit,true, FColor::Red,FColor::Green, 2.0f);		
			}
			else if (CurNotifyState == 9)
			{
				HitMultiplier = 1.0f;
				UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Player->GetActorLocation(),
					Player->GetActorLocation(), 250.f, TraceTypeCustom, true,
					TArray<AActor*>{Player}, EDrawDebugTrace::None,AllThingsHit,true,
					FColor::Red,FColor::Green, 2.0f);
				
				bKb = true;
			}
			break;
		case 4:
			if (CurNotifyState == 0)
			{
				HitMultiplier = 0.0f;
				UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Player->GetActorLocation(),
					Player->GetActorLocation(), 250.f, TraceTypeCustom, true,
					TArray<AActor*>{Player}, EDrawDebugTrace::None,AllThingsHit,true,
					FColor::Red,FColor::Green, 2.0f);

				bAttract = true;
			}
			else if (CurNotifyState == 1)
			{
				HitMultiplier = 0.0f;
				UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Player->GetActorLocation(),
					Player->GetActorLocation(), 225.f, TraceTypeCustom, true,
					TArray<AActor*>{Player}, EDrawDebugTrace::None,AllThingsHit,true,
					FColor::Red,FColor::Green, 2.0f);
            			
				bAttract = true;
			}
			else if (CurNotifyState == 2)
			{
				HitMultiplier = 0.0f;
				UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Player->GetActorLocation(),
					Player->GetActorLocation(), 200.f, TraceTypeCustom, true,
					TArray<AActor*>{Player}, EDrawDebugTrace::None,AllThingsHit,true,
					FColor::Red,FColor::Green, 2.0f);
            			
				bAttract = true;
			}
			else if (CurNotifyState == 3)
			{
				HitMultiplier = 2.0f;
				UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Player->GetActorLocation(),
					Player->GetActorLocation(), 250.f, TraceTypeCustom, true,
					TArray<AActor*>{Player}, EDrawDebugTrace::None,AllThingsHit,true,
					FColor::Red,FColor::Green, 2.0f);

				bKb = true;
			}
			break;
		default:
			break;
		}


	TArray<AActor*> AllActorsHit;
	
	if (!AllThingsHit.IsEmpty())
	{
		for (FHitResult HitResult : AllThingsHit)
		{
			if (!AllActorsHit.Contains(HitResult.GetActor()))
				AllActorsHit.Add(HitResult.GetActor());
		}
	}

	
	if (bAttach)
	{
		if (!AllThingsHit.IsEmpty())
		{
			AActor* ClosestActor = nullptr;
			float ClosestDistance = -1;
			for (AActor* CurActor : AllActorsHit)
			{
				if (CurActor->IsValidLowLevelFast())
				{
					if (CurActor->Tags.Contains("BigEnemy"))
					{
						float DistanceToCheck = (CurActor->GetActorLocation() - Player->GetActorLocation()).Size();
						if (ClosestDistance == -1 || DistanceToCheck < ClosestDistance)
						{
							ClosestActor = CurActor;
							ClosestDistance = DistanceToCheck;
						}
					}
				}
			}
			if (ClosestActor != nullptr)
			{
				FVector JumpVelocity = FVector(0.0f, 0.0f, 0.0f);
				FVector Destination = ClosestActor->GetActorLocation() - Player->GetActorLocation();
				Destination.Normalize();
				Destination = Destination * 150;
				
				UGameplayStatics::SuggestProjectileVelocity_MovingTarget(
					GetWorld(),
					JumpVelocity,
					Player->GetActorLocation(),
					ClosestActor,
					Destination,
					Player->GetCharacterMovement()->GetGravityZ(),
					0.6f,
					EDrawDebugTrace::None,
					2.5f,
					FColor::Red);

				Player->GetCharacterMovement()->StopActiveMovement();
				Player->GetCharacterMovement()->Launch(JumpVelocity);

				FTimerHandle AttachTimer;
				FTimerDelegate AttachDelegate;
				AttachDelegate.BindUFunction(Player, "CheckIfCanAttach", ClosestActor);
				GetWorld()->GetTimerManager().SetTimer(
					AttachTimer,
					AttachDelegate,
					0.6f,
					false);
				return;
			}
			FVector Direction = Player->GetActorForwardVector();
			Direction.Z = 0.2;
			Direction = Direction * 1650.f;
				
			Player->GetCharacterMovement()->StopActiveMovement();
			Player->GetCharacterMovement()->Launch(Direction);
			return;
		}
		FVector Direction = Player->GetActorForwardVector();
		Direction.Z = 0.2;
		Direction = Direction * 1650.f;
				
		Player->GetCharacterMovement()->StopActiveMovement();
		Player->GetCharacterMovement()->Launch(Direction);
		return;
	}
	
	if (!AllActorsHit.IsEmpty())
	{
		if (!bAttract)
			for (AActor* CurActor : AllActorsHit)
			{
				if (CurActor->IsValidLowLevelFast())
				{
					if (bKb)
					{
						if (CurActor->IsValidLowLevelFast())
						{
							FVector Direction = CurActor->GetActorLocation() - Player->GetActorLocation();
							Direction.Z = 0.0f;
							Direction.Normalize();
							Direction.Z = 0.25;
							Direction = Direction * 1250.f;
							
							if (CurActor->FindComponentByClass<UCharacterMovementComponent>())
							{
								CurActor->FindComponentByClass<UCharacterMovementComponent>()->StopActiveMovement();
								CurActor->FindComponentByClass<UCharacterMovementComponent>()->Launch(Direction);
							}
						}
					}
					UGameplayStatics::ApplyDamage(CurActor, Player->GetAttack() * HitMultiplier, Player->GetController(), Player, nullptr);
				}
			}
		else
			for (AActor* CurActor : AllActorsHit)
			{
				if (CurActor->IsValidLowLevelFast())
				{
					FVector Direction = Player->GetActorLocation() - CurActor->GetActorLocation();
					Direction.Z = 0.0f;
					Direction.Normalize();
					Direction.Z = 0.25;
					Direction = Direction * 500.f;
							
					if (CurActor->FindComponentByClass<UCharacterMovementComponent>())
					{
						CurActor->FindComponentByClass<UCharacterMovementComponent>()->StopActiveMovement();
						CurActor->FindComponentByClass<UCharacterMovementComponent>()->Launch(Direction);
					}
				}
			}
	}
}