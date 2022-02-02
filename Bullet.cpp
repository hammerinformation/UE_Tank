
#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	bExploded = false;
	UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleFlash, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector * 1.2f);
	SetLifeSpan(5.0f);
}


void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("DestructibleObject") && bExploded == false) {

		bExploded = true;
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator);
		this->SetLifeSpan(10.0f);


	}
	else if (OtherActor->ActorHasTag("Barrel") && bExploded == false) {
		OtherActor->Destroy();
		FVector OtherActorLocation = OtherActor->GetActorLocation();
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, OtherActorLocation, FRotator::ZeroRotator);

		bExploded = true;
		this->SetLifeSpan(0.0001f);


	}
}

