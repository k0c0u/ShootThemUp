// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"


class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
 
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:
    ASTURifleWeapon();
    
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f; 

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = "Weapon")
    float DamageAmount = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName TraceTargetName = "TraceTarget";
    
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(ReplicatedUsing=OnRep_LastShotInfo)
    FShootInfo LastShotInfo;

    UFUNCTION()
    void OnRep_LastShotInfo();

    virtual void BeginPlay() override;
    
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& Direction) const override;
    
    ACharacter* GetOwnerCharacter() const;

private:
    FTimerHandle ShotTimerHandle;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    void MakeDamage(const FHitResult& HitResult);
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visible);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

    AController* GetController() const;
};
