// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/WeatherComponent.h"

// Sets default values for this component's properties
UWeatherComponent::UWeatherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ParticleComponentWorld = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleComponentWorld"));
	ParticleComponentWorld->SetAutoActivate(false);
	ParticleComponentWorld->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	ParticleComponentWorld->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

	WeatherAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WeatherAudioComponent"));
	WeatherAudioComponent->SetAutoActivate(false);
}


// Called when the game starts
void UWeatherComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	NextRainTime = FMath::FRandRange(RainWaitTimeArray[0], RainWaitTimeArray[1]);
	RainDuration = FMath::FRandRange(RainDurationArray[0], RainDurationArray[1]);
	if(ParticleComponentWorld->IsValidLowLevel())
	{
		ParticleComponentWorld->SetupAttachment(GetOwner()->GetRootComponent());
		ParticleComponentWorld->SetAsset(RainParticleSystem);
	}
	if(WeatherAudioComponent->IsValidLowLevel())
	{
		if(ParticleComponentWorld->IsValidLowLevel())
		{
			WeatherAudioComponent->SetupAttachment(ParticleComponentWorld);
			WeatherAudioComponent->AttachToComponent(ParticleComponentWorld, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			WeatherAudioComponent->SetupAttachment(GetOwner()->GetRootComponent());
		}
	}
}


// Called every frame
void UWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsRaining)
	{
		WaitTime += DeltaTime;
		
		if(WaitTime >= RainDuration)
		{
			StopRain();
		}
	}
	else
	{
		WaitTime += DeltaTime;
		
		if(WaitTime >= NextRainTime)
		{
			StartRain();
		}
	}

	if(ParticleComponentWorld->IsValidLowLevel())
	{
		if(UWFALibrary::GetRPGPlayer()->IsValidLowLevel())
		{
			FVector PlayerLocation = UWFALibrary::GetRPGPlayer()->GetActorLocation();
			FVector ParticleLocation = PlayerLocation + FVector(0.0f, 0.0f, OffSetNiagara);
			ParticleComponentWorld->SetWorldLocation(ParticleLocation);
		}
	}
}

void UWeatherComponent::StartRain()
{	
	bIsRaining = true;
	WaitTime = 0.0f;
	
	NextRainTime = FMath::FRandRange(RainWaitTimeArray[0], RainWaitTimeArray[1]);
	RainDuration = FMath::FRandRange(RainDurationArray[0], RainDurationArray[1]);
	
	if(RainParticleSystem->IsValidLowLevel())
	{
		if(ParticleComponentWorld->IsValidLowLevel())
		{
			ParticleComponentWorld->SetAsset(RainParticleSystem);
			EnableNiagaraEffect();
		}
	}

	SetAudioWeather(RainSound);
	StartWeatherAudio(1.0f);
	GetWorld()->GetTimerManager().ClearTimer(ThunderTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ThunderTimerHandle, this, &UWeatherComponent::SpawnRandomThunderNiagara,FMath::FRandRange(45.0f, 180.0f) , true, 5.0f);
}

void UWeatherComponent::StopRain()
{
	bIsRaining = false;
	WaitTime = 0.0f;

	NextRainTime = FMath::FRandRange(RainWaitTimeArray[0], RainWaitTimeArray[1]);
	RainDuration = FMath::FRandRange(RainDurationArray[0], RainDurationArray[1]);

	DisableNiagaraEffect();
	StopWeatherAudio(2.0f);
	GetWorld()->GetTimerManager().ClearTimer(ThunderTimerHandle);
}

void UWeatherComponent::SetNiagaraEffect(UNiagaraSystem* NewEffect)
{
	if(NewEffect->IsValidLowLevel())
	{
		if(ParticleComponentWorld->IsValidLowLevel())
		{
			DisableNiagaraEffect();
			
			ParticleComponentWorld->SetAsset(NewEffect);
		}
	}
}

void UWeatherComponent::DisableNiagaraEffect()
{	
	ParticleComponentWorld->Deactivate();
}

void UWeatherComponent::EnableNiagaraEffect()
{
	ParticleComponentWorld->Activate(true);
}

void UWeatherComponent::SetAudioWeather(USoundBase* NewSound)
{
	if(NewSound->IsValidLowLevel())
	{
		if(WeatherAudioComponent->IsValidLowLevel())
		{
			StopWeatherAudio(0.5f);
			WeatherAudioComponent->SetSound(NewSound);
		}
	}
}

void UWeatherComponent::StopWeatherAudio(float FadeOutTime)
{	
	if(WeatherAudioComponent->IsValidLowLevel())
	{
		WeatherAudioComponent->FadeOut(FadeOutTime, 0.0f);
	}
}

void UWeatherComponent::ForceStopWeatherAudio()
{
	if(WeatherAudioComponent->IsValidLowLevel())
	{
		WeatherAudioComponent->Stop();
	}
}

void UWeatherComponent::StartWeatherAudio(float FadeInTime)
{	
	if(WeatherAudioComponent->IsValidLowLevel())
	{
		WeatherAudioComponent->FadeIn(FadeInTime, 1.0f);
	}
}

void UWeatherComponent::ForceStartWeatherAudio()
{	
	if(WeatherAudioComponent->IsValidLowLevel())
	{
		WeatherAudioComponent->Play();
	}
}

void UWeatherComponent::SpawnRandomThunderNiagara()
{
	if(bIsRaining)
	{
		if(UWFALibrary::GetRPGPlayer()->IsValidLowLevel())
		{
			FVector PlayerLocation = UWFALibrary::GetRPGPlayer()->GetActorLocation();

			// Faire apparaitre de façon aléatoire le Niagara de l'éclair au alentour du joueur mais jamais proche du joueur
			FVector RandomLocation = PlayerLocation + FVector(FMath::FRandRange(-5000.0f, 5000.0f), FMath::FRandRange(-5000.0f, 5000.0f), 0.0f);
			RandomLocation.Z = PlayerLocation.Z + OffsetThunder;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ThunderParticleSystem, RandomLocation, FRotator(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f), true);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ThunderSound, RandomLocation, FRotator(0.0f, 0.0f, 0.0f), 1.0f, FMath::FRandRange(0.5f, 1.0f));
		}
	}
}