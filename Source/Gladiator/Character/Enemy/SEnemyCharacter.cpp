// Fill out your copyright notice in the Description page of Project Settings.


#include "SEnemyCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gladiator/AbilitySystem/SAttributeSet_Enemy.h"
#include "Gladiator/AI/SAIController.h"


// Sets default values
ASEnemyCharacter::ASEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeSet = CreateDefaultSubobject<USAttributeSet_Enemy>("AttributeSet");
	
}

// Called when the game starts or when spawned
void ASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeSet->OnHealthChanged.AddUObject(this, &ASEnemyCharacter::OnHealthChanged);
	
	AttributeSet->OnDeath.AddUObject(this, &ASEnemyCharacter::OnDeath);
}

// Called every frame
void ASEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* ASEnemyCharacter::GetBehaviorTree()
{
	if (BehaviorTree != nullptr)
	{
		return BehaviorTree;
	}
	return nullptr;
}

void ASEnemyCharacter::OnHealthChanged(AActor* My_Instigator, float ChangeValue)
{
	Super::OnHealthChanged(My_Instigator, ChangeValue);

	if (ChangeValue < 0)
	{
		if (AbilitySystemComp)
		{
			AbilitySystemComp->TryActivateAbilityByClass(GA_Hurt);
		}

		Cast<ASAIController>(GetController())->GetBlackboardComponent()->SetValueAsObject("TargetActor", My_Instigator);
	}
}

void ASEnemyCharacter::OnDeath(AActor* My_Instigator)
{
	Super::OnDeath(My_Instigator);

	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Killed");
	}
	
	GetCharacterMovement()->DisableMovement();
	
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Death);
	}
}

ASProjectileClass* ASEnemyCharacter::GetEnemyProjectileType_Implementation()
{
	if (Projectile != nullptr)
	{
		return Projectile.GetDefaultObject();
	}
	return nullptr;
}

UPaperZDAnimSequence* ASEnemyCharacter::GetAttackAnimSequence_Implementation()
{
	return AttackAnimSequence;
}

