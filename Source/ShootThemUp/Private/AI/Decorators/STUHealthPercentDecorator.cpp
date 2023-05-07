// Shoot Them Up Game. All rights reserved


#include "AI/Decorators/STUHealthPercentDecorator.h"

#include "AIController.h"
#include "STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if(!IsValid(Controller)) return false;

    const auto HealthComponent = Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>();
    if(!IsValid(HealthComponent) || HealthComponent->IsDead()) return false;
    
    return HealthComponent->GetHealthPercent() <= HealthPercent;
}