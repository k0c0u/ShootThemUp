// Shoot Them Up Game. All rights reserved


#include "UI/SpectatorWidget.h"
#include "Components/STURespawnComponent.h"
#include "STUUtils.h"

bool USpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto RespawnComp = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());

    if(!IsValid(RespawnComp) || !RespawnComp->IsRespawnInProgress())
    {
        return false;
    }

    CountDownTime = RespawnComp->GetRespawnCountDown();
    return true;
}
