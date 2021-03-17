// Shoot Them Up Game. All rights reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"



void ASTULauncherWeapon::StartFire() 
{
    MakeShot();
}


void ASTULauncherWeapon::MakeShot() 
{

    if (!GetWorld()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
    //set projectile params

    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

}
