// Shoot Them Up Game. All rights reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"



void ASTULauncherWeapon::StartFire()
{
    Super::StartFire();
    
    MakeShot();
}


void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) return;

    FVector TraceStart, TraceEnd, Direction;
    if (!GetTraceData(TraceStart, TraceEnd, Direction)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector ShotDirection = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(ShotDirection);
        Projectile->FinishSpawning(SpawnTransform);
        Projectile->SetOwner(GetOwner());
    }

    DecreaseAmmo();
    SpawnMuzzleFX();
}
