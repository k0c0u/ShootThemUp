// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "STUGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"


class ASTUPlayerState;
class ASTUGameModeBase;

 
UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    int32 GetKillsNum() const;

    int32 GetCurrentRoundNum() const;

    int32 GetTotalRoundsNum() const;

    int32 GetRoundSecondsRemaining() const;

private:
    ASTUGameModeBase* GetSTUGameMode() const;
    ASTUPlayerState* GetSTUPlayerState() const;
};
