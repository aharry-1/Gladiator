// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "SAttributeSetBase.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, AActor*, float);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GLADIATOR_API USAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	USAttributeSetBase();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USAttributeSetBase, Health);

	FOnValueChanged OnHealthChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USAttributeSetBase, MaxHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(USAttributeSetBase, Attack);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData AttackMultiplier;
	ATTRIBUTE_ACCESSORS(USAttributeSetBase, AttackMultiplier);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attributes")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(USAttributeSetBase, Defence);
};
