// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SBaseCharacter.h"
#include "Gladiator/Interface/SInterface_Enemy.h"
#include "SEnemyCharacter.generated.h"

class UBehaviorTree;

UCLASS()
class GLADIATOR_API ASEnemyCharacter : public ASBaseCharacter, public ISInterface_Enemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviorTree();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GameplayAbility")
	class USAttributeSet_Enemy* AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Hurt;

	UPROPERTY(EditDefaultsOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Death;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GameplayAbility")
	TSubclassOf<ASProjectileClass> Projectile;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	UPaperZDAnimSequence* AttackAnimSequence;

public:
	virtual void OnHealthChanged(AActor* My_Instigator, float ChangeValue) override;

	virtual void OnDeath(AActor* My_Instigator) override;

	virtual ASProjectileClass* GetEnemyProjectileType_Implementation() override;

	virtual UPaperZDAnimSequence* GetAttackAnimSequence_Implementation() override;
};
