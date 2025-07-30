// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackNotify.generated.h"

class APlayers;
/**
 * 
 */
UCLASS()
class WARFORAPPLE_API UAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

	// UPROPERTIES //
public:
	UPROPERTY(EditAnywhere, Category="BoxNumber")
	int CurNotifyState = 0;

protected:

	// UFUNCTIONS //
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
};
