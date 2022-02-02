// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankController.generated.h"

UCLASS()
class MODERNTANK_API ATankController : public APawn
{
	GENERATED_BODY()

public:
	ATankController();
private:

#pragma region Variables

	float MaxSpeed;
	float Speed;
	float MinSpeed;
	float RotateSpeed;


#pragma endregion Variables
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> RightRotatableObjects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> LeftRotatableObjects;
	FTimerHandle FireTimerHandle;


#pragma region Classes
	UPROPERTY(EditAnywhere)
		class AActor* SpawnPoint;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Bullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Turret;
	UPROPERTY(EditAnywhere)
		class UFloatingPawnMovement* FloatingPawnMovement;



#pragma endregion Classes


#pragma region Methods

	void MouseX(float Value);
	void MouseY(float Value);
	void Horizontal(float Value);
	void Vertical(float Value);
	void CameraZoom(float Value);
	void StartFire();
	void StopFire();
	void OnFire();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);
#pragma endregion Methods


};
