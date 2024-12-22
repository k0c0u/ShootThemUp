// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"


UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSoftClassPtr<UUserWidget> PlayerHUDWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSoftClassPtr<UUserWidget> PauseWidgetClass;

    virtual void BeginPlay() override;
    
private:
    void DrawCrossHair();

    UFUNCTION()
    void OnMatchStateChanged(EStuMatchState State);

    UPROPERTY()
    TMap<TEnumAsByte<EStuMatchState>, UUserWidget*> GameWidgets;
    
    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget = nullptr;
};
