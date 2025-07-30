// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Notify/AttackEnemyNotify.h"

#include "AI/AIMobBoss.h"
#include "AI/MassAIEnemy.h"
#include "Character/Players.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UAttackEnemyNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TArray<FHitResult> Hit;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_Pawn) };
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), MeshComp->GetSocketLocation("AttackSocket"), MeshComp->GetSocketLocation("AttackSocket"),
		50.f, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hit, true);

	if (!Hit.IsEmpty())
	{
		for (FHitResult HitResult : Hit)
		{
			if (HitResult.GetActor()->IsValidLowLevelFast())
			{
				if (HitResult.GetActor() != PreviusActor && Cast<APlayers>(HitResult.GetActor()))
				{
					if (!HitResult.GetActor()->Tags.Contains("Enemy"))
					{
						if (AAIMobBase* Enemy = Cast<AAIMobBase>(MeshComp->GetOwner()))
						{
							GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("Hit"));
							UGameplayStatics::ApplyDamage(HitResult.GetActor(), Enemy->GetAttack(), MeshComp->GetOwner()->GetInstigatorController(),
							MeshComp->GetOwner(), UDamageType::StaticClass());
						}

						if ( AMassAIEnemy* Enemy = Cast<AMassAIEnemy>(HitResult.GetActor()))
						{
							GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT("HitMass"));
							UGameplayStatics::ApplyDamage(HitResult.GetActor(), Enemy->GetAttack(), MeshComp->GetOwner()->GetInstigatorController(),
							MeshComp->GetOwner(), UDamageType::StaticClass());
						}
					}
					PreviusActor = HitResult.GetActor();
				}
				
			}
		}
	}
	PreviusActor = nullptr;
}
