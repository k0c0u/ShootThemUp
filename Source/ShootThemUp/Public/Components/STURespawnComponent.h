// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTURespawnComponent();
    
    void Respawn(int32 RespawnTime);

    int32 GetRespawnCountDown() const { return RespawnCountDown; }
    bool IsRespawnInProgress() const { return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimer); }

protected:
	virtual void BeginPlay() override;

private:
    UFUNCTION()
    void RespawnTimerUpdate();
    
    FTimerHandle RespawnTimer;
    int32 RespawnCountDown;
    
};
