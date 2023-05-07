// Shoot Them Up Game. All rights reserved


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
    if(!CanFire()) return;

    if(CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    if(!CanEquip()) return;

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); // берем все выражение по модулю размера массива, чтобы NExtIndex не вышел за его пределы.
    while(NextIndex != CurrentWeaponIndex)
    {
        if(!Weapons[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }

    if(CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}
