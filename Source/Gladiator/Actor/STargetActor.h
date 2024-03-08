// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "Gladiator/Interface/SInterface_Enemy.h"
#include "STargetActor.generated.h"

class UGameplayAbility;
class USAttributeSetBase;
class UAbilitySystemComponent;

UCLASS()
class GLADIATOR_API ASTargetActor : public AActor, public IAbilitySystemInterface, public ISInterface_Enemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTargetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameplayAbilites", meta=(AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, Category="GameplayAbility")
	USAttributeSetBase* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* TargetMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	float AttackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> PreloadedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GameplayAbility")
	TSubclassOf<ASProjectileClass> Projectile;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* My_Instigator, float ChangeValue);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnHealthChanged(AActor* K2_Instigator, float K2_ChangeValue);

	UFUNCTION(BlueprintCallable)
	void AttackTest();

	virtual ASProjectileClass* GetEnemyProjectileType_Implementation() override;
};
