// Shoot Them Up Game. All rights reserved


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    if(!IsValid(PlayerPawn))
    {
        return false;
    }
    const auto HealthComponent = PlayerPawn->FindComponentByClass<USTUHealthComponent>();
    if(!IsValid(HealthComponent) || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = PlayerPawn->FindComponentByClass<USTUWeaponComponent>();
    if(!IsValid(WeaponComponent)) return false;
    
    UE_LOG(LogAmmoPickup, Display, TEXT("Ammo was taken"));
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
