// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gladiator/Framework/SPlayerController.h"


ASBaseCharacter::ASBaseCharacter()
{
	GetCharacterMovement()->GravityScale = 2.2f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->bConstrainToPlane = false;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

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

	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
		if (GE_InitAttributes != nullptr)
		{
			UGameplayEffect* InitAttributes = NewObject<UGameplayEffect>(GetTransientPackage(), GE_InitAttributes);
			AbilitySystemComp->ApplyGameplayEffectToSelf(InitAttributes, 1, EffectContext, PredictionKey);
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
	
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASBaseCharacter::BeginOverlap_HitBox);
}

UAbilitySystemComponent* ASBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ASBaseCharacter::OnHealthChanged(AActor* My_Instigator, float ChangeValue)
{
	if (My_Instigator)
	{
		if (My_Instigator->GetActorLocation().X < GetActorLocation().X)
		{
			LaunchCharacter(FVector(-5.0f, 0.0, 1.0f), true, true);
		}
		else if (My_Instigator->GetActorLocation().X > GetActorLocation().X)
		{
			LaunchCharacter(FVector(5.0f, 0.0, 1.0f), true, true);
		}
	}
	
	K2_OnHealthChanged(My_Instigator, ChangeValue);
}

void ASBaseCharacter::OnDeath(AActor* My_Instigator)
{
	GetCapsuleComponent() -> SetCollisionProfileName("Spectator");
	GetSprite()->SetCollisionProfileName("Spectator");

	K2_OnDeath(My_Instigator);
}

UPaperZDAnimInstance* ASBaseCharacter::GetCharacterAnimInstance_Implementation()
{
	return GetAnimInstance();
}

void ASBaseCharacter::SetHitBox_Implementation(bool bSetActive, FVector Extent, FVector Location)
{
	HitBox->SetBoxExtent(Extent, true);
	HitBox->SetRelativeLocation(Location);
	if (bSetActive)
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

UPaperZDAnimSequence* ASBaseCharacter::GetFiresAnimSequence_Implementation()
{
	return FiresAnimSequence;
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

