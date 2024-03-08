// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetActor.h"

#include "AbilitySystemComponent.h"
#include "Gladiator/AbilitySystem/SAttributeSetBase.h"


// Sets default values
ASTargetActor::ASTargetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>("TargetMesh");
	RootComponent = TargetMesh;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");

	AttributeSet = CreateDefaultSubobject<USAttributeSetBase>("AttributeSet");
}

// Called when the game starts or when spawned
void ASTargetActor::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComp)
	{
		if (PreloadedAbilities.Num() != 0)
		{
			for (auto i : PreloadedAbilities)
			{
				if (i)
				{
					AbilitySystemComp->GiveAbility(i);
				}
			}
		}
	}
	AbilitySystemComp->InitAbilityActorInfo(this, this);

	AttributeSet->OnHealthChanged.AddUObject(this, &ASTargetActor::OnHealthChanged);

	FTimerHandle TimerHandle_AttackDelay;
	
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AttackTest");
	
	AttackDelay = 1.0f;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, true);
	
}

// Called every frame
void ASTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* ASTargetActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ASTargetActor::OnHealthChanged(AActor* My_Instigator, float ChangeValue)
{
	K2_OnHealthChanged(My_Instigator, ChangeValue);
}

void ASTargetActor::AttackTest()
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Attack);
	}
}

ASProjectileClass* ASTargetActor::GetEnemyProjectileType_Implementation()
{
	if (Projectile != nullptr)
	{
		return Projectile.GetDefaultObject();
	}
	return nullptr;
}

