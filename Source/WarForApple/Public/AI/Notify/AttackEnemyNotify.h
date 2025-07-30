// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackEnemyNotify.generated.h"

/**
 * 
 */
UCLASS()
class WARFORAPPLE_API UAttackEnemyNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	AActor* PreviusActor = nullptr;
	
};
