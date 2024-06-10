// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	USTUWeaponComponent();

	virtual void StartFire();
    UFUNCTION(Server, Reliable, Category = "Weapon")
    void Server_StartFire();
    void Server_StartFire_Implementation();
    UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
    void Multicast_StartFire();
    void Multicast_StartFire_Implementation();
    
    void StopFire();
    UFUNCTION(Server, Reliable, Category = "Weapon")
    void Server_StopFire();
    void Server_StopFire_Implementation();
    UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
    void Multicast_StopFire();
    void Multicast_StopFire_Implementation();
    
    virtual void NextWeapon();
    UFUNCTION(Server, Reliable, Category = "Weapon")
    void Server_NextWeapon();
    void Server_NextWeapon_Implementation();
    UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
    void Multicast_NextWeapon();
    void Multicast_NextWeapon_Implementation();
    
    void Reload();
    UFUNCTION(Server, Reliable, Category = "Weapon")
    void Server_Reload();
    void Server_Reload_Implementation();
    UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
    void Multicast_Reload();
    void Multicast_Reload_Implementation();

    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSoftClassPtr<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

    bool IsFiring() const;

    bool NeedAmmo(TSoftClassPtr<ASTUBaseWeapon> WeaponType);

    ACharacter* GetOwnerCharacter() const;

    UFUNCTION()
    void OnRep_CurrentWeapon();
    
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Weapon")
    FName WeponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon)
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY(Replicated)
    TArray<ASTUBaseWeapon*> Weapons;

    UPROPERTY(Replicated)
    int32 CurrentWeaponIndex = 0;
    
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool CanFire() const;
    bool CanEquip() const;
    void EquipWeapon(int32 WeaponIndex);

private:
	UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;
    
	bool EquipAnimInProgress = false;
    
	bool ReloadAnimInProgress = false;

    void SpawnWeapons();

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    
	void PlayAnimMontage(UAnimMontage* Animation);

	void InitAnimation();

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    
    bool CanReload() const;

    void OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();
};
