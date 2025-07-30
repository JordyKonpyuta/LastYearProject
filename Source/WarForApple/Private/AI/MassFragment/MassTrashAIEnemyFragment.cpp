// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MassFragment/MassTrashAIEnemyFragment.h"
#include "MassEntityTemplateRegistry.h"

void UMassEnemyTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment(FConstStructView::Make(EnemyFragment));
	BuildContext.AddTag<FEnemyTag>();
}
