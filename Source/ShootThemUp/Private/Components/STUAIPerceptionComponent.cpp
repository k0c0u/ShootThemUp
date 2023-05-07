// Shoot Them Up Game. All rights reserved


#include "Components/STUAIPerceptionComponent.h"
#include "AI/STUAIController.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "STUUtils.h"

AActor* USTUAIPerceptionComponent::GetClosesEnemy() const
{
    TArray<AActor*> PercieveActors;

    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);

    const auto Controller = Cast<AAIController>(GetOwner());
    if(PercieveActors.Num() == 0 || !IsValid(Controller))
    {
        return nullptr;
    }

    const auto Pawn = Controller->GetPawn();
    if(!IsValid(Pawn))
    {
        return nullptr;
    }

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;
    for(const auto PercieveActor : PercieveActors)
    {
        const auto HealthComponent = PercieveActor->FindComponentByClass<USTUHealthComponent>();

        const auto PercievePawn = Cast<APawn>(PercieveActor);
        const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);
        
        if(IsValid(HealthComponent) && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size(); //the distance to the actor we are observing
            if(CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}
