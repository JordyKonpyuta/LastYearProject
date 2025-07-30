// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIMobBoss.h"

#include "Components/ArrowComponent.h"

AAIMobBoss::AAIMobBoss()
{
	ArrowProjectile = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowProjectile->SetupAttachment(RootComponent);
}
