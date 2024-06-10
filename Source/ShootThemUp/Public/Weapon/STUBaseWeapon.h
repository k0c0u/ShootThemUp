#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"


class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

USTRUCT(BlueprintType)
struct FShootInfo
{
    GENERATED_BODY()

    FShootInfo() : Location_Mul_10(FVector::ZeroVector), Direction(FVector::ZeroVector) {};

    FShootInfo(FVector Location, FVector InDirection) : Location_Mul_10(Location * 10.f), Direction(InDirection) {};
    
    UPROPERTY()
    FVector_NetQuantize100 Location_Mul_10;
    
    UPROPERTY()
    FVector_NetQuantizeNormal Direction;

    FVector GetLocation() const {return Location_Mul_10 * 0.1f;}
    FVector GetDirection() const {return Direction;}
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();

    virtual void StopFire();
    

	void ChangeClip();
    bool CanReload() const;

    FWeaponUIData GetUIData() const {return UIData;}
    FAmmoData GetAmmoData() const {return CurrentAmmo;}

    bool TryToAddAmmo(int32 ClipsAmount);

    bool GetIsFiring() const {return FireInProgress;}

    bool IsAmmoEmpty() const;
    bool IsAmmoFull() const;

protected:
    virtual void BeginPlay() override;
    virtual void MakeShot();

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    FVector GetMuzzleWorldLocation() const;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& Direction) const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();
	
    bool IsClipEmpty() const;
   
    void LogAmmo();

    UNiagaraComponent* SpawnMuzzleFX();

    
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

private:
    UPROPERTY(Replicated)
    FAmmoData CurrentAmmo;

    bool FireInProgress = false;
};