// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerProjectile.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "Gladiator/Character/Player/SPlayerCharacter.h"


// Sets default values
ASPlayerProjectile::ASPlayerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Owner = nullptr;

	OwnerAbilitySystemComp = nullptr;

	AttackMultiplier = 2.0f;
}

// Called when the game starts or when spawned
void ASPlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASPlayerProjectile::OnProjectileOverlap);
	
}

void ASPlayerProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Owner->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
	{
		OwnerAbilitySystemComp = Cast<UAbilitySystemComponent>(Owner->GetComponentByClass(UAbilitySystemComponent::StaticClass()));
		if (OwnerAbilitySystemComp && OtherActor && OtherActor != Owner && OtherActor != this && OtherActor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(OtherActor->GetComponentByClass(UAbilitySystemComponent::StaticClass()));

			UGameplayEffect* EffectInstance = Cast<UGameplayEffect>(DamageGameplayEffect->GetDefaultObject());
			if (ensureMsgf(EffectInstance, TEXT("DamageGameplayEffect没有设置！")))
			{
				if (ASPlayerCharacter* PlayerCharacter = Cast<ASPlayerCharacter>(Owner))
				{
					PlayerCharacter->SetPlayerAttackMultiplier_Implementation(AttackMultiplier);
				}
				OwnerAbilitySystemComp->ApplyGameplayEffectToTarget(EffectInstance, ASC);
				Destroy();
			}
		}
	}
}

// Called every frame
void ASPlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

