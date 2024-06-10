// Shoot Them Up Game. All rights reserved


#include "UI/STUPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"


bool USTUPauseWidget::Initialize()
{
    const auto InitStatus =  Super::Initialize();
    if(IsValid(ClearPauseButton))
    {
        ClearPauseButton->OnClicked.AddUniqueDynamic(this, &USTUPauseWidget::OnClearPause);
    }
    
    return InitStatus;
}

void USTUPauseWidget::OnClearPause()
{
    if(!IsValid(GetWorld()) || !IsValid(GetWorld()->GetAuthGameMode())) return;
    GetWorld()->GetAuthGameMode()->ClearPause();
}
