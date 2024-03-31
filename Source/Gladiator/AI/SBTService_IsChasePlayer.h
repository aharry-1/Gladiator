// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_IsChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATOR_API USBTService_IsChasePlayer : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Setting")
	FName IsChasePlayerKey;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	FName PlayerActorKey;

	UPROPERTY(EditAnywhere, Category="Setting")
	float ChaseDistance;
	
};
