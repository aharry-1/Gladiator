// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTaskNode_EmptyPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gladiator/Widget/SWorldWidget.h"

EBTNodeResult::Type USBTTaskNode_EmptyPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(PlayerLocationKey, FVector(0));
		BlackboardComp->SetValueAsBool(LostPlayerKey, true);
		
		if (LostPlayerUI)
		{
			USWorldWidget* LostPlayerWidget = Cast<USWorldWidget>(BlackboardComp->GetValueAsObject(WorldWidgetKey));
			if (LostPlayerWidget != nullptr)
			{
				LostPlayerWidget->RemoveFromParent();
			}
			
			APawn* PlayPawn = Cast<APawn>(BlackboardComp->GetValueAsObject(PlayerActorKey));
			if (PlayPawn)
			{
				APlayerController* PlayerController = Cast<APlayerController>(PlayPawn->GetController());
				if (PlayerController)
				{
					LostPlayerWidget = CreateWidget<USWorldWidget>(PlayerController, LostPlayerUI);
					if (LostPlayerWidget)
					{
						LostPlayerWidget->AttachedActor = OwnerComp.GetAIOwner()->GetPawn();

						LostPlayerWidget->WorldOffset = BlackboardComp->GetValueAsVector(UIWorldOffsetKey);
						
						LostPlayerWidget->AddToViewport();

						BlackboardComp->SetValueAsObject(WorldWidgetKey, LostPlayerWidget);
					}
				}
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
