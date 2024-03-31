// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_FindMoveLocation.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATOR_API USBTService_FindMoveLocation : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	FName MoveLocationKey;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	float MinDistance = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	float MaxDistance = 1000.0f;
	
};
