// Shoot Them Up Game. All rights reserved

#pragma once

#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;


UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

public:
    ASTUPlayerCharacter(const FObjectInitializer& ObjInit);

    //virtual void Tick(float DeltaTime) override;
    
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual bool IsRunning() const override;

    UFUNCTION(BlueprintCallable, Category = "Aim")
    FRotator GetPlayerControlRotation();

protected:
    virtual void BeginPlay() override;
    virtual void OnDeath() override;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void OnStartRunning();
    void OnStopRunning();
    void OnStartFire();

    UFUNCTION()
    void OnCameraCollisionComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    UFUNCTION()
    void OnCameraCollisionComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlap();
    

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* CameraCollisionComponent;
    
private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    UPROPERTY(Replicated)
    FRotator AimRotation = FRotator::ZeroRotator;
};
