// Shoot Them Up Game. All rights reserved


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"


USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto ImpactData = DefaultImpactData;

    if(Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if(ImpactDataMap.Contains(PhysMat))
        {
            ImpactData = ImpactDataMap[PhysMat];
        }
    }

    //niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
        ImpactData.NiagaraEffect,                              //
        Hit.ImpactPoint,                                       //
        Hit.ImpactNormal.Rotation());

    //decal
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //
        ImpactData.DecalData.Material,                 //
        ImpactData.DecalData.Size,                     //
        Hit.ImpactPoint,                               //
         Hit.ImpactNormal.Rotation());

    if(IsValid(DecalComponent))
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}


void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
	
}



void USTUWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
}

