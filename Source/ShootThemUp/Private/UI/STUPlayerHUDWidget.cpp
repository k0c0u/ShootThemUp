// Shoot Them Up Game. All rights reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

bool USTUPlayerHUDWidget::Initialize()
{
    if (IsValid(GetOwningPlayerPawn()))
    {
        GetOwningPlayerPawn()->ReceiveControllerChangedDelegate.AddUniqueDynamic(this, &USTUPlayerHUDWidget::OnReceiveControllerChanged);
        OnReceiveControllerChanged(GetOwningPlayerPawn(), GetOwningPlayer(), GetOwningPlayer());

        HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUHealthComponent>();
        WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<USTUWeaponComponent>();
    }
    return Super::Initialize();
}


float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if(!HealthComponent) return 0.0f;
    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if(!WeaponComponent) return false;
    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if(!WeaponComponent) return false;
    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    if(IsValid(GetOwningPlayerPawn()) && IsValid(HealthComponent))
    {
        return HealthComponent && !HealthComponent->IsDead();
    }
    return false;
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const APlayerController* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if(HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
}

void USTUPlayerHUDWidget::OnReceiveControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
    if(!IsValid(HealthComponent))
    {
        HealthComponent = Pawn->FindComponentByClass<USTUHealthComponent>();
    }
   
    if(IsValid(HealthComponent) && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChanged);
    }
}
