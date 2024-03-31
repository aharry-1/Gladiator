// Fill out your copyright notice in the Description page of Project Settings.


#include "SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Gladiator/Character/Enemy/SEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	
	UBehaviorTree* BehaviorTree = Cast<ASEnemyCharacter>(GetPawn())->GetBehaviorTree();
	
	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
	}

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		// GetBlackboardComponent()->SetValueAsVector("TargetLocation", MyPawn->GetActorLocation());
	
		GetBlackboardComponent()->SetValueAsObject("PlayerActor", MyPawn);
	}
}

void ASAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("TargetLocation", MyPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
}

void ASAIController::SetUI_WorldOffset(FVector Offset)
{
	GetBlackboardComponent()->SetValueAsVector("UIWorldOffset", Offset);
}
