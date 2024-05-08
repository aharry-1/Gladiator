// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "../SBaseCharacter.h"
#include "Gladiator/Interface/SInterface_Player.h"
#include "SPlayerCharacter.generated.h"

UENUM()
enum ESCacheInput
{
	Empty,
	Attack,
	Jump,
	Dash
};

/**
 * 
 */
UCLASS()
class GLADIATOR_API ASPlayerCharacter : public ASBaseCharacter, public ISInterface_Player
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ASPlayerCharacter();

	virtual void OnHealthChanged(AActor* My_Instigator, float ChangeValue) override;

	virtual void OnDeath(AActor* My_Instigator) override;

	UFUNCTION(BlueprintCallable)
	void OnManaChanged(AActor* My_Instigator, float ChangeValue);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnManaChanged(AActor* K2_Instigator, float K2_ChangeValue);

	UFUNCTION(BlueprintCallable, Category = "SeamlessTravel")
	void SeamlessTravelTo(FString URL);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_SwitchLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCloseWall;

	UFUNCTION()
	void WallDetectionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void EndWallSlide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void WallSlide();
	
	UFUNCTION()
	void JumpWallSlide();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName JumpToNodeName_JumpWallSlide;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Falling() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void BeginOverlap_HitBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* FiresAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	class UInputAction* ShowUIAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* WallDetection;
	
	void UpdateControllerRotation();

	void Move(const FInputActionValue& Value);
	
	UPROPERTY()
	class UEnhancedInputLocalPlayerSubsystem* InputSubSystem;

protected:
	//GAS相关
	UPROPERTY(BlueprintReadOnly, Category="GameplayAbility")
	class USAttributeSet_Player* AttributeSet;
	
	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void Fires();
	
	UPROPERTY()
	bool bIsAttack;

	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GameplayAbility")
	// float AttackInterval;

	// UPROPERTY()
	// UAbilityAsync_WaitGameplayTagRemoved* WaitAttackAsync;
	// UFUNCTION()
	// void WaitEndAttack();
	//
	// UFUNCTION(BlueprintCallable)
	// void EndAttack();
	// void ClearWaitAttack();
	
	void MyJump();
	
	void MyStopJumping();
	
	void Dash();

	void ShowUI();
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	// TSubclassOf<UGameplayEffect> GE_Ready_NormalAttack_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TArray<TSubclassOf<UGameplayAbility>> GA_Attacks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_StopJumping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Dash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Fires;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Hurt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayAbility> GA_Death;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayAbility")
	TSubclassOf<UGameplayEffect>  GE_GetFires;

	UPROPERTY()
	bool bFirstSetAttackCache;

	UPROPERTY()
	TEnumAsByte<ESCacheInput> CacheInput;

public:
	//ISInterface_Player函数
	virtual APlayerController* GetPlayerCharacterController_Implementation() override;

	virtual void SetCache_Implementation() override;

	virtual void DoCache_Implementation() override;

	virtual void ClearCache_Implementation() override;

	virtual void SetPlayerAttackMultiplier_Implementation(float AttackMultiplier) override;

	virtual void SwitchLevel_Implementation() override;
};
