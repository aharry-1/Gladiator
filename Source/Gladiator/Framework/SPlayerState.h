// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Gladiator/Interface/SInterface_Player.h"
#include "SPlayerState.generated.h"

class ASPlayerProjectile;

/**
 * 
 */
UCLASS()
class GLADIATOR_API ASPlayerState : public APlayerState, public ISInterface_Player
{
	GENERATED_BODY()

public:
	ASPlayerState();
		
	virtual ASPlayerProjectile* GetProjectileType_Implementation() override;

	virtual void SetProjectileType_Implementation(TSubclassOf<ASPlayerProjectile> Projectile) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ASPlayerProjectile* ProjectileType;
};
