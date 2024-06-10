// Shoot Them Up Game. All rights reserved


#include "STUGameModeBase.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Player/STUPlayerController.h"
#include "Player/STUBaseCharacter.h"
#include "UI/STUGameHUD.h"
#include "Player/STUPlayerState.h"
#include "STURespawnComponent.h"
#include "STUUtils.h"

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase() 
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();

}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    CurrentRound = 1;

    // Another method of calling
    /*UAssetManager* AssetManager = &UAssetManager::Get();
    SpawnBotHandle = AssetManager->GetStreamableManager().RequestAsyncLoad();*/
    
    FStreamableManager& AssetLoader = UAssetManager::GetStreamableManager();
    SpawnBotHandle = AssetLoader.RequestAsyncLoad(AIControllerClass.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ThisClass::SpawnBots));

    SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if(IsValid(InController) && InController->IsA<AAIController>() && !AICPawnClass.IsNull())
    {
        return AICPawnClass.LoadSynchronous();
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if(IsValid(KillerPlayerState))
    {
        KillerPlayerState->AddKill();
    }

    if(IsValid(VictimPlayerState))
    {
        VictimPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if(PauseSet)
    {
        SetMatchState(ESTUMatchState::Pause);
    }
    
    return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseSet =  Super::ClearPause();
    if(PauseSet)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseSet;
}

void ASTUGameModeBase::SpawnBots()
{
    const auto Controller = Cast<UClass>(SpawnBotHandle->GetLoadedAsset());
    if(!IsValid(Controller))
    {
        return;
    }
    
    if(!IsValid(GetWorld()))
    {
        return;
    }
    
    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(Controller, SpawnInfo);
        RestartPlayer(STUAIController);
    }
    CreateTeamsInfo();
    StartRound();
    
    //GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::CreateTeamsInfo);
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ThisClass::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogTemp, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
    
    if(--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if(CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if(!IsValid(GetWorld()))
    {
        return;
    }

    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if(IsValid(Controller) && IsValid(Controller->GetPawn()))
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if(!IsValid(GetWorld()))
    {
        return;
    }

    int32 TeamID = 1;
    
    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!IsValid(Controller))
        {
           continue;
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!IsValid(PlayerState))
        {
            continue;
        }

        PlayerState->SetTeamID(TeamID);
        const FLinearColor Color = DetermineColorByTeamID(TeamID);
        PlayerState->SetTeamColor(Color);
        SetPlayerColor(Controller);
        
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if(TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if(!IsValid(Controller))
    {
        return;
    }

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if(!IsValid(Character))
    {
        return;
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if(!IsValid(PlayerState))
    {
        return;
    }

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if(IsValid(GetWorld()))
    {
        return;
    }

    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!IsValid(Controller))
        {
            continue;
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!IsValid(PlayerState))
        {
            continue;
        }

        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if(!RespawnAvailable) return;
    
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if(!IsValid(RespawnComponent)) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
    
}

void ASTUGameModeBase::GameOver()
{
    LogPlayerInfo();

    for(auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if(IsValid(Pawn))
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if(MatchState == State)
    {
        return;
    }

    MatchState = State;
    OnMatchStateChanged.Broadcast(State);
}
