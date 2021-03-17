// Shoot Them Up Game. All rights reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"


USTUWeaponComponent::USTUWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	WeponSocketPointName = "WeaponSocket";

    CurrentWeapon = nullptr;
}


// Called when the game starts
void USTUWeaponComponent::BeginPlay()

{
	Super::BeginPlay();

	SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
    if (!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeponSocketPointName);
    CurrentWeapon->SetOwner(Character);//Передаем Character в класс STUBaseWeapon, чтобы в нем получить GetOwner.
}

void USTUWeaponComponent::StartFire() 
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}