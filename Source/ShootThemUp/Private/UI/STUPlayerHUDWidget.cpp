// Shoot Them Up Game. All rights reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const USTUHealthComponent* HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
    if(!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const USTUWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUWeaponComponent>();
    if(!WeaponComponent) return false;
    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const USTUWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUWeaponComponent>();
    if(!WeaponComponent) return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    if(GetOwningPlayerPawn())
    {
        const USTUHealthComponent* HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
        return HealthComponent && !HealthComponent->IsDead();
    }
    return false;
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const APlayerController* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize()
{
    if (!IsValid(GetOwningPlayerPawn())) return Super::Initialize();
    
    USTUHealthComponent* HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
    if(IsValid(HealthComponent))
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
    }
    
    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if(HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
}
