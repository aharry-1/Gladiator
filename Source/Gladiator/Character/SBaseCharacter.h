// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "PaperZDCharacter.h"
#include "SBaseCharacter.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GLADIATOR_API ASBaseCharacter : public APaperZDCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASBaseCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> MyAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GameplayAbilities)
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> GE_InitAttributes;

protected:
	
	UPROPERTY()
	FVector SpriteDefaultLocation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector SpriteOffset;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* HitBox;

	UFUNCTION()
	virtual void BeginOverlap_HitBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;
};
