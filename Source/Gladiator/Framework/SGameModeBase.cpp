// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// void ASGameModeBase::SetLevelPlayerASC(bool IsWin, AActor* Player)
// {
// 	USGameInstance* GameInstance = Cast<USGameInstance>(GetGameInstance());
// 	
// 	GameInstance->IsWin = IsWin;
// 	
// 	if (IsWin)
// 	{
// 		UAbilitySystemComponent* PlayerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player);
// 		if (PlayerASC)
// 		{
// 			TArray<FGameplayAbilitySpecHandle> AbilityHandles;
// 			PlayerASC->GetAllAbilities(AbilityHandles);
// 			for (auto Ability : AbilityHandles)
// 			{
// 				bool bIsInstance;
//
// 				GameInstance->ASC.Abilities.Add(*NewObject<UGameplayAbility>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(PlayerASC, Ability, bIsInstance)->GetClass()));
// 			}
//
// 			TArray<FGameplayAttribute> Attributes;
// 			PlayerASC->GetAllAttributes(Attributes);
// 			for (auto Attribute : Attributes)
// 			{
// 				GameInstance->ASC.Attributes.Add(Attribute);
// 				
// 				bool bSuccessfully;
// 				GameInstance->ASC.AttributeValues.Add(UAbilitySystemBlueprintLibrary::GetFloatAttributeFromAbilitySystemComponent(PlayerASC, Attribute, bSuccessfully));
// 			}
//
// 			FGameplayTagContainer TagContainer;
// 			PlayerASC->GetOwnedGameplayTags(TagContainer);
// 			for (auto Tag : TagContainer)
// 			{
// 				GameInstance->ASC.Tags.Add(Tag);
// 			}
// 		}
// 	}
// }


ASGameModeBase::ASGameModeBase()
{
	bUseSeamlessTravel = true;
}

void ASGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASGameModeBase::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToTransition, ActorList);
	
	GetSaveToNextLevelActors(ActorList);
}

void ASGameModeBase::GetSaveToNextLevelActors_Implementation(TArray<AActor*>& ActorList)
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		
	UKismetSystemLibrary::PrintString(this,FString("ASGameModeBase::GetSaveToNextLevelActors"),false,true);
}

void ASGameModeBase::K2_ServerTravel_Implementation(const FString& URL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	
	//World->ServerTravel(URL, bAbsolute, bShouldSkipGameNotify);
	GetWorld()->GetFirstPlayerController()->ClientTravel(URL, ETravelType::TRAVEL_Relative, true);
}