// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Gladiator/Actor/Projectile/SPlayerProjectile.h"

ASPlayerState::ASPlayerState()
{
	ProjectileType = nullptr;
}

ASPlayerProjectile* ASPlayerState::GetProjectileType_Implementation()
{
	if (ProjectileType != nullptr)
	{
		return ProjectileType;
	}
	return nullptr;
}

void ASPlayerState::SetProjectileType_Implementation(TSubclassOf<ASPlayerProjectile> Projectile)
{
	if (Projectile)
	{
		ProjectileType = Projectile.GetDefaultObject();
	}
}

