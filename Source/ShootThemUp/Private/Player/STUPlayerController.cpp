// Shoot Them Up Game. All rights reserved


#include "Player/STUPlayerController.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.h"
#include "STURespawnComponent.h"
#include "GameFramework/GameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if(!IsValid(InputComponent))
    {
        return;
    }

    InputComponent->BindAction("Pause", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
}

void ASTUPlayerController::OnPauseGame()
{
    if(!IsValid(GetWorld()) || !IsValid(GetWorld()->GetAuthGameMode()))
    {
        return;
    }

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnMatchStateChanged(EStuMatchState State)
{
    if(State == EStuMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
