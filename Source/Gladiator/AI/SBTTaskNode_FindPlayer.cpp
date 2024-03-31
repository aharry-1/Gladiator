// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTaskNode_FindPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gladiator/Widget/SWorldWidget.h"


EBTNodeResult::Type USBTTaskNode_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(LostPlayerKey, false);
		
		if (FindPlayerUI)
		{
			USWorldWidget* FindPlayerWidget = Cast<USWorldWidget>(BlackboardComp->GetValueAsObject(WorldWidgetKey));
			if (FindPlayerWidget != nullptr)
			{
				FindPlayerWidget->RemoveFromParent();
			}
			
			APawn* PlayPawn = Cast<APawn>(BlackboardComp->GetValueAsObject(PlayerActorKey));
			if (PlayPawn)
			{
				APlayerController* PlayerController = Cast<APlayerController>(PlayPawn->GetController());
				if (PlayerController)
				{
					FindPlayerWidget = CreateWidget<USWorldWidget>(PlayerController, FindPlayerUI);
					if (FindPlayerWidget)
					{
						FindPlayerWidget->AttachedActor = OwnerComp.GetAIOwner()->GetPawn();

						FindPlayerWidget->WorldOffset = BlackboardComp->GetValueAsVector(UIWorldOffsetKey);
						
						FindPlayerWidget->AddToViewport();

						BlackboardComp->SetValueAsObject(WorldWidgetKey, FindPlayerWidget);
					}
				}
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
