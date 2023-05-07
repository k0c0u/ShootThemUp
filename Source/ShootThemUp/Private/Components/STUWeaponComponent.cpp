// Shoot Them Up Game. All rights reserved


#include "Components/STUWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "Animation/EquipFinishedAnimNotify.h"
#include "Animation/STUReloadFinishedAnimNotify.h"
#include "Animation/AnimUtils.h"
#include "GameFramework/Character.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    
    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);
    
    CurrentWeaponIndex = 0;
	SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
    InitAnimation();
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
    CurrentWeapon = nullptr;

    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }

    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    if (!GetWorld()) return;

    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!IsValid(Character)) return;

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty);
        Weapon->SetOwner(GetOwner());

        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;

    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) 
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!IsValid(Character)) return;

    if (CurrentWeapon)
    {
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeponArmorySocketName);
        CurrentWeapon->StopFire();
    }
    
    CurrentWeapon = Weapons[WeaponIndex];

    //CurrentReloadAnimMintage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                   //
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire() 
{
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon() 
{
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) 
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimation()
{
     const auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimMontage);
     if (EquipFinishedNotify)
     {
         EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
     }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }
    
     for (const auto OneWeaponData : WeaponData)
     {
         const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
         if (!ReloadFinishedNotify)
         {
             UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
             checkNoEntry();
         }
         ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
     }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent) 
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    ReloadAnimInProgress = false;
}
bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload() 
{
    ChangeClip();
}

void USTUWeaponComponent::OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon)
{
    if(!IsValid(AmmoEmptyWeapon)) return;
    if(CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip();
    }
    else
    {
        for(const auto Weapon : Weapons)
        {
            if(Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}

void USTUWeaponComponent::ChangeClip() 
{
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if(CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if(CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSoftClassPtr<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for(const auto Weapon : Weapons)
    {
       if(IsValid(Weapon)) // ? && WeaponType.Get()->IsChildOf(Weapon->GetClass()) || Weapon->GetClass()->IsChildOf(WeaponType.Get()) ||  Weapon->IsA(WeaponType.Get())
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

bool USTUWeaponComponent::IsFiring() const
{
    return IsValid(CurrentWeapon) && CurrentWeapon->GetIsFiring();
}

bool USTUWeaponComponent::NeedAmmo(TSoftClassPtr<ASTUBaseWeapon> WeaponType)
{
    for(const auto Weapon : Weapons)
    {
        if(IsValid(Weapon) && Weapon->IsA(WeaponType.Get())) // ?
            {
                return !Weapon->IsAmmoFull();
            }
    }
    return false;
}