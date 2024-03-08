// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileClass.h"
#include "SPlayerProjectile.generated.h"


UCLASS()
class GLADIATOR_API ASPlayerProjectile : public ASProjectileClass
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPlayerProjectile();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAbilitySystemComponent* OwnerAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AttackMultiplier;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
