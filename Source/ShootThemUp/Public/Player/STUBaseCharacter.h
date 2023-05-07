// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "PaintColor";

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void OnDeath();

public:	

	UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetPlayerColor(const FLinearColor& Color);

private:

	bool WantsToRun = false;
    bool IsMovingForward = false;

	void MoveForward(float Amount);
    void MoveRight(float Amount);

    void OnStartRunning();
    void OnStopRunning();

    void OnStartFire();

	void OnHealthChanged(float Health, float HealthDelta);

	UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

	
};
