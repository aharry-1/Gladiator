// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASBaseCharacter::ASBaseCharacter()
{
	GetCharacterMovement()->GravityScale = 2.2f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->bConstrainToPlane = false;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	
	SpriteOffset = GetSprite()->GetRelativeLocation();
	
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	HitBox->SetupAttachment(GetSprite());
	HitBox->SetBoxExtent(FVector(0.0f), false);
	
}

void ASBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.OnGround"));

	//赋予GA
	if(AbilitySystemComp)
	{
		if(MyAbilities.Num() > 0)
		{
			for(auto i = 0; i < MyAbilities.Num(); i++)
			{
				if(MyAbilities[i]==nullptr)
				{
					continue;
				}
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(MyAbilities[i], 1, 0));
			}
		}
		
		FGameplayEffectContextHandle EffectContext;
		FPredictionKey PredictionKey;
		UGameplayEffect* InitAttributes = NewObject<UGameplayEffect>(GetTransientPackage(), GE_InitAttributes);
		AbilitySystemComp->ApplyGameplayEffectToSelf(InitAttributes, 1, EffectContext, PredictionKey);
		
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASBaseCharacter::BeginOverlap_HitBox);
}

UAbilitySystemComponent* ASBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ASBaseCharacter::BeginOverlap_HitBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
	{
		if (AbilitySystemComp)
		{
			UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(OtherActor->GetComponentByClass(UAbilitySystemComponent::StaticClass()));

			UGameplayEffect* EffectInstance = Cast<UGameplayEffect>(DamageGameplayEffect->GetDefaultObject());
			if (ensureMsgf(EffectInstance, TEXT("DamageGameplayEffect没有设置！")))
			{
				AbilitySystemComp->ApplyGameplayEffectToTarget(EffectInstance, ASC);
			}
		}
	}
}

