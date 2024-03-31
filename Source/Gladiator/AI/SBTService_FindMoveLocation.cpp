// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_FindMoveLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

void USBTService_FindMoveLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		AActor* AIActor = Cast<AActor>(BlackboardComp->GetValueAsObject("SelfActor"));
		FVector TargetLocation = AIActor->GetActorLocation();;
		
		float Distance = FMath::RandRange(MinDistance, MaxDistance);
		TargetLocation.X += FMath::RandBool() ? Distance : -Distance;

		BlackboardComp->SetValueAsVector(MoveLocationKey, TargetLocation);
	}
}
