// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    ASTUPlayerController();

protected:
    virtual void BeginPlay() override;
    //virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTURespawnComponent* STURespawnComponent;

private:
    void OnPauseGame();
    
    UFUNCTION()
    void OnMatchStateChanged(EStuMatchState State);
};
