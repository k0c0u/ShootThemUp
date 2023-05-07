// Shoot Them Up Game. All rights reserved


#include "AI/Tasks/BTTaskNewLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTaskNewLocation::UBTTaskNewLocation()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type UBTTaskNewLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
    if(!IsValid(Controller) || !IsValid(Blackboard) || !IsValid(Pawn))
    {
        return EBTNodeResult::Failed;
    }

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if(!IsValid(NavSys))
    {
        return EBTNodeResult::Failed;
    }
    
    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();
    if(!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if(!IsValid(CenterActor)) return EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }
    
    const bool Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if(!Found)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}
