// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeSetBase.h"
#include "SAttributeSet_Player.generated.h"


/**
 * 
 */
UCLASS()
class GLADIATOR_API USAttributeSet_Player : public USAttributeSetBase
{
	GENERATED_BODY()
	
public:
	USAttributeSet_Player();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USAttributeSet_Player, MaxMana);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USAttributeSet_Player, Mana);

	FOnValueChanged OnManaChanged;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
};
