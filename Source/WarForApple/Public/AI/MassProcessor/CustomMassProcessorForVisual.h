// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassEntitySubsystem.h"
#include "CustomMassProcessorForVisual.generated.h"

/**
 * 
 */
UCLASS()
class WARFORAPPLE_API UCustomMassProcessorForVisual : public UMassProcessor
{
	GENERATED_BODY()

public:
	UCustomMassProcessorForVisual();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
	
};
