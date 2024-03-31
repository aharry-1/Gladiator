// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gladiator/Actor/Projectile/SPlayerProjectile.h"
#include "UObject/Interface.h"
#include "SInterface_Player.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USInterface_Player : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GLADIATOR_API ISInterface_Player
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	APlayerController* GetPlayerCharacterController();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	void SetCache();

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	// void SetAttackCacheEnd();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	void DoCache();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	void ClearCache();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	ASPlayerProjectile* GetProjectileType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	void SetProjectileType(TSubclassOf<ASPlayerProjectile> Projectile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="PlayerInterface")
	void SetPlayerAttackMultiplier(float AttackMultiplier);
};
