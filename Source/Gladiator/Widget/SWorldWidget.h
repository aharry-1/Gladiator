﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class GLADIATOR_API USWorldWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

    UPROPERTY(meta=(BindWidget))
    USizeBox* ParentSizeBox;
    
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
    FVector WorldOffset;
    
    UPROPERTY(BlueprintReadWrite, Category="UI", meta=(ExposeOnSpawn=true))
    AActor* AttachedActor;
};
