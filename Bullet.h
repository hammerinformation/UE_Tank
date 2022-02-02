// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class MODERNTANK_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

public:
	

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BulletMesh;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* ExplosionEffect;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* MuzzleFlash;

	bool bExploded = false;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);
};
