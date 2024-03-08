// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileClass.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PaperFlipbookComponent.h"


// Sets default values
ASProjectileClass::ASProjectileClass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileClass::OnProjectileHit);
	RootComponent = SphereComp;

	FlightAudioComp = CreateDefaultSubobject<UAudioComponent>("FlightAudioComp");
	FlightAudioComp->SetupAttachment(SphereComp);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	
	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComp");
	FlipbookComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp -> bRotationFollowsVelocity = false;
	MovementComp -> bInitialVelocityInLocalSpace = true;
	MovementComp -> ProjectileGravityScale = 0.0f;
	MovementComp -> InitialSpeed = 500;
}

void ASProjectileClass::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(this))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), GetActorRotation());

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitAudioComp, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), 0.0f, 3000.0f, 2.0f, true);
		
		Explode();
	}
}

void ASProjectileClass::Explode_Implementation()
{
	Destroy();
}

