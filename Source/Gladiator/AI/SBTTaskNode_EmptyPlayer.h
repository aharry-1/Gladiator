// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskNode_EmptyPlayer.generated.h"

class USWorldWidget;
/**
 * 
 */
UCLASS()
class GLADIATOR_API USBTTaskNode_EmptyPlayer : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category="Setting")
	FName PlayerLocationKey;

	UPROPERTY(EditAnywhere, Category="Setting")
	FName LostPlayerKey;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	FName PlayerActorKey;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	FName UIWorldOffsetKey;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	FName WorldWidgetKey;
	
	UPROPERTY(EditAnywhere, Category="Setting")
	TSubclassOf<USWorldWidget> LostPlayerUI;
};
