// Shoot Them Up Game. All rights reserved


#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"


USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    
}


// Called when the game starts
void USTURespawnComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if(!IsValid(GetWorld())) return;

    RespawnCountDown = RespawnTime;

    GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RespawnTimerUpdate, 1.0f);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if(RespawnCountDown == 0)
    {
        if(!IsValid(GetWorld())) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);

        const auto GameMode = Cast<ASTUGameModeBase>(GetOwner());
        if(!IsValid(GameMode)) return;

        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}

