// Shoot Them Up Game. All rights reserved


#include "Pickups/STUHealthPickup.h"

#include "STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    if(!IsValid(PlayerPawn))
    {
        return false;
    }
    const auto HealthComponent = PlayerPawn->FindComponentByClass<USTUHealthComponent>();
    if(!IsValid(HealthComponent) || HealthComponent->IsDead()) return false;

    UE_LOG(LogHealthPickup, Display, TEXT("Health was taken"));
    return HealthComponent->TryToAddHealth(HealthAmount);
}
