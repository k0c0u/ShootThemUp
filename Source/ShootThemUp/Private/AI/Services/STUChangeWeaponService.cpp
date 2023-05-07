// Shoot Them Up Game. All rights reserved


#include "AI/Services/STUChangeWeaponService.h"
#include "AIController.h"
#include "STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if(Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
        if(WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
        {
            WeaponComponent->NextWeapon();
        }
    }
    
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
