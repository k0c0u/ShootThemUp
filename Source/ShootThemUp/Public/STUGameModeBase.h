// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

struct FStreamableHandle;
class AAIController;


UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASTUGameModeBase();
    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    void Killed(AController* KillerController, AController* VictimController);

    FGameData GetGameData() const {return GameData;}
    int32 GetCurrentRoundNum() const {return CurrentRound;}
    int32 GetRoundSecondsRemaining() const {return RoundCountDown;}

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSoftClassPtr<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSoftClassPtr<APawn> AICPawnClass;
    
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    TSharedPtr<FStreamableHandle> SpawnBotHandle;

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;
    
    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(AController* Controller);

    void LogPlayerInfo();
};
