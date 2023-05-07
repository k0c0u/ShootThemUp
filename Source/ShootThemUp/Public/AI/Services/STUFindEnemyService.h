// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BTService.h"
#include "STUFindEnemyService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
    USTUFindEnemyService();
protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;
};
