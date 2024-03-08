// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SInterface_PayerController.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USInterface_PayerController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GLADIATOR_API ISInterface_PayerController
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PayerControllerInterface")
	void InitHealth(float MaxHealth);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PayerControllerInterface")
	void OnHealthChange(float Health, float ChangeValue);
};
