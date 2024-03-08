// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeSetBase.h"

#include "GameplayEffectExtension.h"

USAttributeSetBase::USAttributeSetBase():
	Health(100.0f),
	MaxHealth(100.0f),
	Attack(10.0f),
	AttackMultiplier(1.0f),
	Defence(10.0f)
{
}

void USAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void USAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		OnHealthChanged.Broadcast(Data.EffectSpec.GetContext().GetInstigator(), Data.EvaluatedData.Magnitude);
	}
}
