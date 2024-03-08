// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Gladiator/Interface/SInterface_AttributeUI.h"


ASPlayerController::ASPlayerController()
{
	
}

void ASPlayerController::BeginPlay()
{
	if (ensureMsgf(UI_Attribute, TEXT("UI_Attribute没有设置")))
	{
		AttributeUI = CreateWidget<UUserWidget>(this, UI_Attribute);
		AttributeUI->AddToViewport();
		AttributeUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ensureMsgf(UI_Health, TEXT("UI_Health没有设置")))
	{
		HealthUI = CreateWidget<UUserWidget>(this, UI_Health);
		HealthUI->AddToViewport();
	}
}

void ASPlayerController::ShowAttributeUI()
{
	if (AttributeUI != nullptr)
	{
		if (AttributeUI->GetVisibility() == ESlateVisibility::Collapsed)
		{
			AttributeUI->SetVisibility(ESlateVisibility::Visible);

			if (UI_Attribute->ImplementsInterface(USInterface_AttributeUI::StaticClass()))
			{
				ISInterface_AttributeUI::Execute_ShowUI(AttributeUI, GetPawn());
			}
		}
		else
		{
			AttributeUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
