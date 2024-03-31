// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskNode_ExecuteGameplayAbility.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class GLADIATOR_API USBTTaskNode_ExecuteGameplayAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	UPROPERTY(EditAnywhere, Category="Setting")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
};
