// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Library/WFALibrary.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"
#include "WeatherComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UWFALibrary;
class UAudioComponent;
class USoundBase;
class UNiagaraFunctionLibrary;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class WARFORAPPLE_API UWeatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeatherComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Thunder")
	UNiagaraSystem* ThunderParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Thunder")
	USoundBase* ThunderSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Rain")
	UNiagaraSystem* RainParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Rain")
	USoundBase* RainSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Component")
	UNiagaraComponent* ParticleComponentWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Component")
	UAudioComponent* WeatherAudioComponent;

	UPROPERTY(EditAnywhere, Blueprintable, Category = "Weather - Rain")
	bool bIsRaining = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Logic")
	float WaitTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Rain")
	float RainDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Rain")
	float NextRainTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Rain")
	TArray<float> RainDurationArray = {60.0f, 300.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Rain")
	TArray<float> RainWaitTimeArray = {100.0f, 900.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Logic")
	float OffSetNiagara = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather - Logic")
	float OffsetThunder = 2500.0f;

	UPROPERTY()
	FTimerHandle ThunderTimerHandle;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void StartRain();

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void StopRain();

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void SetNiagaraEffect(UNiagaraSystem* NewEffect);

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void DisableNiagaraEffect();

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void EnableNiagaraEffect();

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void SetAudioWeather(USoundBase* NewSound);

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void StartWeatherAudio(float FadeInTime);

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void ForceStartWeatherAudio();

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void StopWeatherAudio(float FadeOutTime);

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void ForceStopWeatherAudio();

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void SpawnRandomThunderNiagara();
		
};
