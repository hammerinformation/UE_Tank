
#include "TankController.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ATankController::ATankController()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	RootComponent = Body;

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(-40.0f, 0, 90.0f));
	//	SpringArm->SetRelativeRotation(FRotator(0.0f, -30.0f, 0.0f));
	SpringArm->TargetArmLength = 800.0f;

	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	Turret->SetupAttachment(RootComponent);
	Turret->SetRelativeRotation(FRotator::ZeroRotator);
	FloatingPawnMovement = CreateDefaultSubobject< UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	bUseControllerRotationYaw = true;


}

void ATankController::BeginPlay()
{
	Super::BeginPlay();


	SpawnPoint->AttachToComponent(Turret, FAttachmentTransformRules::KeepWorldTransform);
	Body->OnComponentHit.AddDynamic(this, &ATankController::OnHit);

}

void ATankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATankController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("StartFire", IE_Pressed, this, &ATankController::StartFire);
	PlayerInputComponent->BindAction("StartFire", IE_Pressed, this, &ATankController::OnFire);
	PlayerInputComponent->BindAction("StopFire", IE_Released, this, &ATankController::StopFire);


	PlayerInputComponent->BindAxis("Mouse X", this, &ATankController::MouseX);
	PlayerInputComponent->BindAxis("Mouse Y", this, &ATankController::MouseY);
	PlayerInputComponent->BindAxis("Vertical", this, &ATankController::Vertical);
	PlayerInputComponent->BindAxis("Horizontal", this, &ATankController::Horizontal);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ATankController::CameraZoom);

}

void ATankController::MouseX(float Value)
{
	if (Value != 0) {
		SpringArm->AddRelativeRotation(FRotator(0, Value * 2, 0));
		Turret->AddRelativeRotation(FRotator(0, Value * 2, 0));
	}
}

void ATankController::MouseY(float Value)
{
	if (Value != 0) {
		SpringArm->AddRelativeRotation(FRotator(Value * 2, 0, 0));
	}
}

void ATankController::Horizontal(float Value)
{
	if (Value != 0) {
		AddControllerYawInput(Value / 4);
	}
}

void ATankController::Vertical(float Value)
{
	if (Value != 0) {
		AddMovementInput(GetActorForwardVector(), Value);
		Value *= 2.0f;
		for (auto& Whell : RightRotatableObjects)
		{
			if (Whell) {
				Whell->AddActorLocalRotation(FRotator(Value, 0, 0));

			}
		}

		for (auto& Whell : LeftRotatableObjects)
		{
			if (Whell) {
				Whell->AddActorLocalRotation(FRotator(-1.0f * Value, 0, 0));

			}
		}

	}
}

void ATankController::CameraZoom(float Value)
{
	if (Value != 0) {
		auto TargetArmLength = SpringArm->TargetArmLength + (Value * 50);
		SpringArm->TargetArmLength = TargetArmLength;
		if (SpringArm->TargetArmLength >= 1500.0f) { SpringArm->TargetArmLength = 1500.0f; }
		else if (SpringArm->TargetArmLength <= 400.0f) { SpringArm->TargetArmLength = 400.0f; }
	}
}

void ATankController::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATankController::OnFire, 1.0f, true);

}

void ATankController::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ATankController::OnFire()
{
	FVector Location = SpawnPoint->GetActorLocation();
	FRotator Rotation = Turret->GetComponentRotation();
	AActor* TankBullet = GetWorld()->SpawnActor<AActor>(Bullet, Location, Rotation);
	if (TankBullet) {

		if (TankBullet->FindComponentByClass<UStaticMeshComponent>()) {
			auto BulletSpeed = 20000000.0f * 3.0f;
			TankBullet->FindComponentByClass<UStaticMeshComponent>()->AddForce(Turret->GetForwardVector() * BulletSpeed);
		}
	}
}

void ATankController::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}


