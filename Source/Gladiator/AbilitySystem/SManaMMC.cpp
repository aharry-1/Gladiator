// Fill out your copyright notice in the Description page of Project Settings.


#include "SManaMMC.h"

#include "SGameplayAbility.h"

float USManaMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const USGameplayAbility* Ability = Cast<USGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}

	return Ability->ManaCost.GetValueAtLevel(Ability->GetAbilityLevel());
}
