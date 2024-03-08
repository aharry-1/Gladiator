// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeSet_Player.h"

#include "GameplayEffectExtension.h"

USAttributeSet_Player::USAttributeSet_Player():
	MaxMana(100.0f),
	Mana(100.0f)
{
}

void USAttributeSet_Player::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void USAttributeSet_Player::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));

		OnManaChanged.Broadcast(Data.EffectSpec.GetContext().GetInstigator(), Data.EvaluatedData.Magnitude);
	}
}
