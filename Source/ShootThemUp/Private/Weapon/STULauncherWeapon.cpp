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

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->FinishSpawning(SpawnTransform);
        Projectile->SetOwner(GetOwner());
    }

    DecreaseAmmo();
    SpawnMuzzleFX();
}
