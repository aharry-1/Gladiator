// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GLADIATOR_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();

	virtual void BeginPlay() override;

	virtual void GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList) override;

// 	UFUNCTION(BlueprintCallable)
// 	void SetLevelPlayerASC(bool IsWin, AActor* Player);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetSaveToNextLevelActors(TArray<AActor*>& ActorList);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void K2_ServerTravel(const FString& URL, bool bAbsolute, bool bShouldSkipGameNotify);
};
