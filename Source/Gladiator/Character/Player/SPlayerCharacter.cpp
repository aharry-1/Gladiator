// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerCharacter.h"


#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gladiator/AbilitySystem/SAttributeSet_Player.h"
#include "Gladiator/Framework/SGameInstance.h"
#include "Gladiator/Framework/SPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ASPlayerCharacter::ASPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	AttributeSet = CreateDefaultSubobject<USAttributeSet_Player>("AttributeSet");

	WallDetection = CreateDefaultSubobject<UBoxComponent>("WallDetection");
	WallDetection->SetupAttachment(RootComponent);
	WallDetection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WallDetection->SetCollisionObjectType(ECC_WorldStatic);
	
	bIsCloseWall = false;
	WallDetection->OnComponentBeginOverlap.AddDynamic(this, &ASPlayerCharacter::WallDetectionOverlap);
	WallDetection->OnComponentEndOverlap.AddDynamic(this, &ASPlayerCharacter::EndWallSlide);
	
	bFirstSetAttackCache = true;

	JumpToNodeName_JumpWallSlide = "JumpWallSlide";

	//SpriteOffset = FVector(42.0f, 0.0f, 21.9f);
}

void ASPlayerCharacter::OnHealthChanged(AActor* My_Instigator, float ChangeValue)
{
	Super::OnHealthChanged(My_Instigator, ChangeValue);
	
	if (ChangeValue < 0)
	{
		if (AbilitySystemComp)
		{
			AbilitySystemComp->TryActivateAbilityByClass(GA_Hurt);
		}
	}
}

void ASPlayerCharacter::OnDeath(AActor* My_Instigator)
{
	Super::OnDeath(My_Instigator);

	APlayerController* PC = Cast<APlayerController>(GetController());
	DisableInput(PC);
	
	Cast<ASPlayerController>(GetController())->HealthUI->RemoveFromParent();
	
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Death);
	}
}

void ASPlayerCharacter::OnManaChanged(AActor* My_Instigator, float ChangeValue)
{
	K2_OnManaChanged(My_Instigator, ChangeValue);
}

void ASPlayerCharacter::SeamlessTravelTo(FString URL)
{
	if (IsLocallyControlled())
	{
		GetPlayerCharacterController_Implementation()->ClientTravel(URL, ETravelType::TRAVEL_Relative, true);
	}
}

// Called when the game starts or when spawned
void ASPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::PrintString(this,FString("ASPlayerCharacter::BeginPlay()"),false,true);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubSystem)
		{
			InputSubSystem->AddMappingContext(MappingContext, 0);
		}
	}

	SpriteDefaultLocation = GetSprite()->GetRelativeLocation();

	AttributeSet->OnHealthChanged.AddUObject(this, &ASPlayerCharacter::OnHealthChanged);

	AttributeSet->OnManaChanged.AddUObject(this, &ASPlayerCharacter::OnManaChanged);
	
	AttributeSet->OnDeath.AddUObject(this, &ASPlayerCharacter::OnDeath);

	// //更换关卡时同步ASC
	// USGameInstance* GameInstance = Cast<USGameInstance>(GetGameInstance());
	// if (GameInstance != nullptr)
	// {
	// 	if (GameInstance->IsWin)
	// 	{
	// 		if (AbilitySystemComp)
	// 		{
	// 			for (auto Ability : GameInstance->ASC.Abilities)
	// 			{
	// 				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(Ability.GetClass(), 1, 0));
	// 			}
	// 			
	// 			for (int i=0; GameInstance->ASC.Attributes[i] != nullptr; i++)
	// 			{
	// 				FGameplayAttribute Attribute = GameInstance->ASC.Attributes[i];
	// 				
	// 				UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>();
	// 				GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	// 				
	// 				FGameplayModifierInfo Modifier;
	// 				Modifier.Attribute = Attribute;
	// 				Modifier.ModifierOp = EGameplayModOp::Override;
	// 				Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(GameInstance->ASC.AttributeValues[i]);
	//
	// 				GameplayEffect->Modifiers.Add(Modifier);
	// 				FGameplayEffectContextHandle EffectContext;
	// 				
	// 				AbilitySystemComp->ApplyGameplayEffectToSelf(GameplayEffect, 1, EffectContext);
	// 			}
	//
	// 			for (auto Tag : GameInstance->ASC.Tags)
	// 			{
	// 				AbilitySystemComp->AddLooseGameplayTag(Tag);
	// 			}
	// 		}
	// 	}
	// }
	
	//UAbilityAsync_WaitGameplayTagRemoved::WaitGameplayTagRemoveFromActor(this, FGameplayTag::RequestGameplayTag("Ready.NormalAttack"))->Removed.AddDynamic(this, &ASPlayerCharacter::ASPlayerCharacter::WaitEndAttack);
	//WaitAttackAsync->Removed.AddDynamic(this, &ASPlayerCharacter::ASPlayerCharacter::WaitEndAttack);
}

void ASPlayerCharacter::WallDetectionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		bIsCloseWall = true;
	}
	//UE_LOG(LogTemp, Warning, TEXT("BeginWallDetectionBoxOverlap"));
}

void ASPlayerCharacter::JumpWallSlide()
{
	if (GetMovementComponent()->Velocity.Z <= 100.0f &&
		InputSubSystem && InputSubSystem->GetPlayerInput() &&
		InputSubSystem->GetPlayerInput()->FindActionInstanceData(MoveAction) &&
		InputSubSystem->GetPlayerInput()->FindActionInstanceData(MoveAction)->GetTriggerEvent() == ETriggerEvent::Triggered &&
		AbilitySystemComp && !AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WallSlide")))
	{
		WallSlide();
	}
}

void ASPlayerCharacter::WallSlide()
{
	AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WallSlide"));
	
	GetCharacterMovement()->Velocity.Z = 0.0f;
	GetCharacterMovement()->GravityScale = 0.05f;

	GetAnimInstance()->JumpToNode(JumpToNodeName_JumpWallSlide);

	GetSprite()->SetRelativeLocation(SpriteOffset);
}

void ASPlayerCharacter::EndWallSlide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (AbilitySystemComp)
		{
			AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WallSlide"));
		}
		
		bIsCloseWall = false;
		GetCharacterMovement()->GravityScale = 2.2f;
		
		GetSprite()->SetRelativeLocation(SpriteDefaultLocation);
	}
}

void ASPlayerCharacter::Falling()
{
	Super::Falling();

	if (AbilitySystemComp)
	{
		if (!AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.OnAir")))
		{
			AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.OnAir"));
		}
		AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.OnGround"));
	}

	WallDetection->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (AbilitySystemComp)
	{
		if (!AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.OnGround")))
		{
			AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.OnGround"));
		}
		AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.OnAir"));
	}
	
	WallDetection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASPlayerCharacter::BeginOverlap_HitBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap_HitBox(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this && OtherActor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
	{
		if (AbilitySystemComp)
		{
			//击退
			FVector KnockBackDirection = -GetActorForwardVector();
			FRotator CapsuleRotator = GetCapsuleComponent()->GetRelativeRotation();
			KnockBackDirection.Z = KnockBackDirection.X * sin(CapsuleRotator.Pitch);
			CapsuleRotator.Pitch >= 0 ? KnockBackDirection.Z : KnockBackDirection.Z = -KnockBackDirection.Z;
			
			float KnockBackForce;
			if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.OnGround")))
			{
				KnockBackForce = 200.0f;
			}
			else
			{
				KnockBackForce = 100.0f;
			}
			KnockBackDirection *= KnockBackForce;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *KnockBackDirection.ToString());
			
			LaunchCharacter(KnockBackDirection, false, false);
		}
		else
		{
			AbilitySystemComp->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.AttackWall"));
		}
	}
}

// Called every frame
void ASPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateControllerRotation();

	if (bIsCloseWall)
	{
		JumpWallSlide();
	}

	// FString aa;
	// if (InputSubSystem && InputSubSystem->GetPlayerInput() && InputSubSystem->GetPlayerInput()->FindActionInstanceData(MoveAction))
	// {
	// 	switch (InputSubSystem->GetPlayerInput()->FindActionInstanceData(MoveAction)->GetTriggerEvent()) {
	// 	case ETriggerEvent::None:
	// 		aa = "None";
	// 		break;
	// 	case ETriggerEvent::Triggered:
	// 		aa = "Triggered";
	// 		break;
	// 	case ETriggerEvent::Started:
	// 		aa = "Started";
	// 		break;
	// 	case ETriggerEvent::Ongoing:
	// 		aa = "Ongoing";
	// 		break;
	// 	case ETriggerEvent::Canceled:
	// 		aa = "Canceled";
	// 		break;
	// 	case ETriggerEvent::Completed:
	// 		aa = "Completed";
	// 		break;
	// 	default: aa = "default";
	// 	}
	// }
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *aa);
	
}

void ASPlayerCharacter::UpdateControllerRotation()
{
	if (AController* PlayerController = GetController())
	{
		//前进方向
		if (!AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.OnAttack")))
		{
			float Velocity = GetCharacterMovement()->Velocity.X;
			if (Velocity > 0)
			{
				PlayerController->SetControlRotation(FRotator(0.0f));
			}
			if (Velocity < 0)
			{
				PlayerController->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
			}
		}

		//斜面坡度
		FVector NormalVector = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
		float MyAngle = UKismetMathLibrary::DegAcos(FVector::DotProduct(FVector(0.0f, 0.0f, 1.0f), NormalVector));
		if (PlayerController->GetControlRotation().Yaw < 90.0f && NormalVector.X > 0)
		{
			MyAngle = -MyAngle;
		}
		if (PlayerController->GetControlRotation().Yaw > 90.0f && NormalVector.X < 0)
		{
			MyAngle = -MyAngle;
		}
		
		FRotator CapsuleRotator = GetCapsuleComponent()->GetRelativeRotation();
		if (GetCharacterMovement()->IsWalking() && MyAngle < 45 && MyAngle > -45)
		{
			GetCapsuleComponent()->SetRelativeRotation(FRotator(MyAngle, CapsuleRotator.Yaw, CapsuleRotator.Roll));
		}
		else
		{
			GetCapsuleComponent()->SetRelativeRotation(FRotator(0, CapsuleRotator.Yaw, CapsuleRotator.Roll));
		}
	}
}

// Called to bind functionality to input
void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//jump
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ASPlayerCharacter::MyJump);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASPlayerCharacter::MyStopJumping);

		//move
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASPlayerCharacter::Move);

		//Attack
		EnhancedInputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &ASPlayerCharacter::Attack);

		//Fires
		EnhancedInputComp->BindAction(FiresAction, ETriggerEvent::Started, this, &ASPlayerCharacter::Fires);

		//Dash
		EnhancedInputComp->BindAction(DashAction, ETriggerEvent::Started, this, &ASPlayerCharacter::Dash);
		
		//ShowUI
		EnhancedInputComp->BindAction(ShowUIAction, ETriggerEvent::Started, this, &ASPlayerCharacter::ShowUI);
	}
}

void ASPlayerCharacter::Move(const FInputActionValue& Value)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag("State.OnAttack"));
	if (AbilitySystemComp->HasAnyMatchingGameplayTags(TagContainer))
	{
		return;
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	AddMovementInput(FVector(1.0, 0.0, 0.0), MovementVector.X);
}

void ASPlayerCharacter::Attack()
{
	if (AbilitySystemComp)
	{
		if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WallSlide")))
		{
			//弹射一小段距离防止贴墙
			if (GetActorRotation().Vector().X > 0)
			{
				LaunchCharacter(FVector(-50.0f, 0.0, 10.0f), true, true);
			}
			if (GetActorRotation().Vector().X <= 0)
			{
				LaunchCharacter(FVector(50.0f, 0.0, 10.0f), true, true);
			}
			//转身
			if (AController* PlayerController = GetController())
			{
				if (GetActorRotation().Vector().X <= 0)
				{
					PlayerController->SetControlRotation(FRotator(0.0f));
				}
				if (GetActorRotation().Vector().X > 0)
				{
					PlayerController->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
				}
			}
		}
		for (auto Attack : GA_Attacks)
		{
			bIsAttack = AbilitySystemComp->TryActivateAbilityByClass(Attack);
			if (bIsAttack)
			{
				break;
			}
		}

		// if (!bIsAttack && AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.OnAttack")))
		// {
		// 	AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack"));
		//
		// 	FTimerHandle WaitAttackTimerHandle;
		// 	GetWorld()->GetTimerManager().SetTimer(WaitAttackTimerHandle, this, &ASPlayerCharacter::ClearWaitAttack, AttackInterval, false);
		// }
	}
}

void ASPlayerCharacter::Fires()
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Fires);
	}
}

// if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.OnAttack")))
// {
// 	WaitAttackAsync = UAbilityAsync_WaitGameplayTagRemoved::WaitGameplayTagRemoveFromActor(this, FGameplayTag::RequestGameplayTag("State.OnAttack"), true);
// 	WaitAttackAsync->Removed.AddDynamic(this, &ASPlayerCharacter::WaitAttack);
// 		
// 	AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack"));
//
// 	FTimerHandle WaitAttackTimerHandle;
// 	GetWorld()->GetTimerManager().SetTimer(WaitAttackTimerHandle, this, &ASPlayerCharacter::ClearWaitAttack, WaitAttackTime, false);
// }

// void ASPlayerCharacter::WaitEndAttack()
// {
// 	FTimerHandle WaitAttackTimerHandle;
// 	GetWorld()->GetTimerManager().SetTimer(WaitAttackTimerHandle, this, &ASPlayerCharacter::EndAttack, AttackInterval, false);
// }

// void ASPlayerCharacter::EndAttack()
// {
// 	if (AbilitySystemComp)
// 	{
// 		if (!AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Ready.NormalAttack")))
// 		{
// 			UGameplayEffect* NormalAttack = NewObject<UGameplayEffect>(GetTransientPackage(), GE_Ready_NormalAttack_1);
// 			AbilitySystemComp->ApplyGameplayEffectToSelf(NormalAttack, 1, FGameplayEffectContextHandle(), FPredictionKey());
// 		}
// 		
// 		// if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack")))
// 		// {
// 		// 	Attack();
// 		// 	UE_LOG(LogTemp, Warning, TEXT("WaitAttack"));
// 		// }
// 	}
// }

//void ASPlayerCharacter::ClearWaitAttack()
//{
//	AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack"));
//}

void ASPlayerCharacter::MyJump()
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Jump);
	}
}


void ASPlayerCharacter::MyStopJumping()
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_StopJumping);
	}
}

void ASPlayerCharacter::Dash()
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Dash);
	}
}

void ASPlayerCharacter::ShowUI()
{
	ASPlayerController* PlayerController = Cast<ASPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->ShowAttributeUI();
	}
}

APlayerController* ASPlayerCharacter::GetPlayerCharacterController_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		return PlayerController;
	}
	return nullptr;
}

void ASPlayerCharacter::SetCache_Implementation()
{
	if (InputSubSystem && InputSubSystem->GetPlayerInput())
	{
		ETriggerEvent Attack = InputSubSystem->GetPlayerInput()->FindActionInstanceData(AttackAction)->GetTriggerEvent();
		ETriggerEvent Dash = InputSubSystem->GetPlayerInput()->FindActionInstanceData(DashAction)->GetTriggerEvent();
		ETriggerEvent Jump = InputSubSystem->GetPlayerInput()->FindActionInstanceData(JumpAction)->GetTriggerEvent();
		
		if (bFirstSetAttackCache)
		{
			if (Attack == ETriggerEvent::Completed || Dash == ETriggerEvent::Completed || Jump == ETriggerEvent::Completed)
			{
				bFirstSetAttackCache = false;
			}
		}
		else
		{
			if (Attack == ETriggerEvent::Triggered)
			{
				CacheInput = ESCacheInput::Attack;
			}
			if (Dash == ETriggerEvent::Triggered)
			{
				CacheInput = ESCacheInput::Dash;
			}
			if (Jump == ETriggerEvent::Triggered)
			{
				CacheInput = ESCacheInput::Jump;
			}
		}
	}
}

// void ASPlayerCharacter::SetAttackCacheEnd_Implementation()
// {
// 	if (AbilitySystemComp)
// 	{
// 		switch (CacheInput) {
// 		case ESCacheInput::Attack:
// 			if (!AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack")))
// 			{
// 				//AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack"));
// 				Attack();
// 			}
// 			break;
// 		case ESCacheInput::Dash:
// 			if (!AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitDash")))
// 			{
// 				//AbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitDash"));
// 				Dash();
// 			}
// 			break;
// 		default: ;
// 		}
// 	}
// }

void ASPlayerCharacter::DoCache_Implementation()
{
	
	switch (CacheInput)
	{
	case ESCacheInput::Attack:
		Attack();
		
		break;
	case ESCacheInput::Dash:
		Dash();
		
		break;
	case ESCacheInput::Jump:
		Jump();
		
		break;
	case ESCacheInput::Empty:
		break;
	default: ;
	}
	
	// if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack")))
	// {
	// 	InputSubSystem->InjectInputVectorForAction(AttackAction, FVector(1), TArray<UInputModifier*>(), TArray<UInputTrigger*>());
	// }
	// if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitDash")))
	// {
	// 	InputSubSystem->InjectInputVectorForAction(DashAction, FVector(1), TArray<UInputModifier*>(), TArray<UInputTrigger*>());
	// }
}

void ASPlayerCharacter::ClearCache_Implementation()
{
	// if (AbilitySystemComp)
	// {
	// 	AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitAttack"));
	// 	AbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.WaitDash"));
	// }
	bFirstSetAttackCache = true;
	CacheInput = ESCacheInput::Empty;
}

void ASPlayerCharacter::SetPlayerAttackMultiplier_Implementation(float AttackMultiplier)
{
	AttributeSet->SetAttackMultiplier(AttackMultiplier);
}

void ASPlayerCharacter::SwitchLevel_Implementation()
{
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	if (PlayerStart != nullptr)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
	}

	FGameplayEffectContextHandle EffectContext;
	FPredictionKey PredictionKey;
	if (GE_InitAttributes != nullptr)
	{
		UGameplayEffect* InitAttributes = NewObject<UGameplayEffect>(GetTransientPackage(), GE_InitAttributes);
		AbilitySystemComp->ApplyGameplayEffectToSelf(InitAttributes, 1, EffectContext, PredictionKey);
	}
	if (AbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("GameplayEffect.HaveFires")))
	{
		UGameplayEffect* InitAttributes = NewObject<UGameplayEffect>(GetTransientPackage(), GE_GetFires);
		AbilitySystemComp->ApplyGameplayEffectToSelf(InitAttributes, 1, EffectContext, PredictionKey);
	}
	
	K2_SwitchLevel();
}
