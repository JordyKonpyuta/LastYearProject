// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC/NPC_Character.h"

#include "Library/WFALibrary.h"

// Sets default values
ANPC_Character::ANPC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanRotate)
	{
		SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UWFALibrary::GetRPGPlayer()->GetActorLocation()), DeltaTime, 200.0f));
		if (FMath::Abs(FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UWFALibrary::GetRPGPlayer()->GetActorLocation()).Yaw)) < 1.f)
			bCanRotate = false;
	}
}

// Called to bind functionality to input
void ANPC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Character::OnInteract_Implementation()
{
	IInterface_NPC::OnInteract_Implementation();
	bCanRotate = true;
	UWFALibrary::GetRPGPlayer()->bIsEndDialogue = false;
	UWFALibrary::GetRPGPlayer()->MoveCameraInDialogue();
	DisplayWidget();
}

void ANPC_Character::SetActiveDialogueIndex_Implementation()
{
}

void ANPC_Character::SetDialogueLines_Implementation()
{
}

void ANPC_Character::DisplayWidget_Implementation()
{
}



