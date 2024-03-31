// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Gladiator/Actor/Projectile/SProjectileClass.h"
#include "UObject/Interface.h"
#include "SInterface_Enemy.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USInterface_Enemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GLADIATOR_API ISInterface_Enemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="EnemyInterface")
	ASProjectileClass* GetEnemyProjectileType();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="EnemyInterface")
	UPaperZDAnimSequence* GetAttackAnimSequence();
};
