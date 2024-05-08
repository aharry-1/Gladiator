// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Gladiator/Interface/SInterface_PayerController.h"

#include "SPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class GLADIATOR_API ASPlayerController : public APlayerController, public ISInterface_PayerController
{
	GENERATED_BODY()

public:
	ASPlayerController();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void ShowAttributeUI();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UUserWidget> UI_Attribute;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UUserWidget> UI_Health;
		
	UPROPERTY()
	UUserWidget* AttributeUI;

public:
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HealthUI;
	
};
