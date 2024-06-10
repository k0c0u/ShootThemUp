// Shoot Them Up Game. All rights reserved


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::OnRep_LastShotInfo()
{
    SpawnTraceFX(GetMuzzleWorldLocation(), LastShotInfo.Location_Mul_10);
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}
void ASTURifleWeapon::StartFire()
{
    Super::StartFire();

    InitMuzzleFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceStart, TraceEnd, ShootDirection;
    if (!GetTraceData(TraceStart, TraceEnd, ShootDirection))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;
    
    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;

        if(GetOwner()->HasAuthority())
        {
            MakeDamage(HitResult);
        }

        if(GetOwnerCharacter()->IsLocallyControlled())
        {
            WeaponFXComponent->PlayImpactFX(HitResult);
        }
    }
    
    if(GetOwner()->HasAuthority())
    {
        LastShotInfo.Location_Mul_10 = TraceFXEnd;
    }

    if(GetOwnerCharacter()->IsLocallyControlled())
    {
        SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
    }
    
    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd, FVector& Direction) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    
    return true;
}

ACharacter* ASTURifleWeapon::GetOwnerCharacter() const
{
    return Cast<ACharacter>(GetOwner());
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;
    DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if(!IsValid(MuzzleFXComponent))
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
    if(IsValid(MuzzleFXComponent))
    {
        MuzzleFXComponent->SetPaused(!Visible);
        MuzzleFXComponent->SetVisibility(Visible, true);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if(IsValid(TraceFXComponent))
    {
        TraceFXComponent->SetVariableVec3(TraceTargetName, TraceEnd);
    }
}

AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

void ASTURifleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASTURifleWeapon, DamageAmount);
    DOREPLIFETIME_CONDITION(ASTURifleWeapon, LastShotInfo, COND_SkipOwner);
    /*
    FDoRepLifetimeParams RepParams;
    RepParams.Condition = COND_SimulatedOnly;
    RepParams.RepNotifyCondition = REPNOTIFY_Always;
    DOREPLIFETIME_WITH_PARAMS(ASTURifleWeapon, LastShotInfo, RepParams)*/
}