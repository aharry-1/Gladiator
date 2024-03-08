// Fill out your copyright notice in the Description page of Project Settings.


#include "SDamageGE_ExecutionCalculation.h"

#include "SAttributeSetBase.h"

struct FSDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence);

	FSDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USAttributeSetBase, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USAttributeSetBase, Attack, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USAttributeSetBase, AttackMultiplier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USAttributeSetBase, Defence, Target, false);
	}
};

static const FSDamageStatic& DamageStatic()
{
	static  FSDamageStatic DmgStatic;
	return DmgStatic;
}

USDamageGE_ExecutionCalculation::USDamageGE_ExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatic().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatic().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatic().AttackMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatic().DefenceDef);
}

void USDamageGE_ExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	//获得ASC
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	//获得Actor
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//获得Tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//设置属性
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().HealthDef, EvaluationParameters, Health);
	
	float Attack = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AttackDef, EvaluationParameters,	Attack);

	float AttackMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().AttackMultiplierDef, EvaluationParameters, AttackMultiplier);

	float Defence = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DefenceDef, EvaluationParameters, Defence);

	float DamageDone = Attack * (Attack/(Attack+Defence)) * AttackMultiplier;

	// if (TargetTags->HasTag(FGameplayTag::RequestGameplayTag("Ability.Buff.Block")))
	// {
	// 	DamageDone *= 0.5f;
	// }

	DamageDone = FMath::RoundToInt(DamageDone);
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatic().HealthProperty, EGameplayModOp::Additive, -DamageDone));
	}
}
