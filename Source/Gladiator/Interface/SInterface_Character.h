// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "UObject/Interface.h"
#include "SInterface_Character.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USInterface_Character : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GLADIATOR_API ISInterface_Character
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="CharacterInterface")
	void SetHitBox(bool bSetActive, FVector Extent, FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="CharacterInterface")
	UPaperZDAnimInstance* GetCharacterAnimInstance();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="EnemyInterface")
	UPaperZDAnimSequence* GetFiresAnimSequence();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="EnemyInterface")
	void SwitchLevel();
};
