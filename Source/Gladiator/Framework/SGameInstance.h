// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Engine/GameInstance.h"
#include "SGameInstance.generated.h"

// USTRUCT()
// struct FPlayerASC
// {
// 	GENERATED_BODY()
// 	
// 	TArray<UGameplayAbility> Abilities;
// 	TArray<FGameplayAttribute> Attributes;
// 	TArray<float> AttributeValues;
// 	TArray<FGameplayTag> Tags;
// };


/**
 * 
 */
UCLASS()
class GLADIATOR_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=GameplayAbilities)
	bool IsWin = false;

	// UPROPERTY(VisibleAnywhere, Category=GameplayAbilities)
	// TArray<UGameplayAbility> Abilities;
	
};
