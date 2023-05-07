// Shoot Them Up Game. All rights reserved


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNum() const
{
    return 0;
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
    return 0;
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
    return 0;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
    return 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()) : nullptr;
}
