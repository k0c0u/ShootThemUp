// Shoot Them Up Game. All rights reserved


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "STUGameModeBase.h"

void ASTUGameHUD::DrawHUD() 
{
    Super::DrawHUD();

    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();
    
    if(!IsValid(GetWorld()))
    {
        return;
    }
    
    GameWidgets.Add(ESTUMatchState::InProgress,CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass.LoadSynchronous()));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass.LoadSynchronous()));

    for(auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if(!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Collapsed);
    };
    
    if(const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
    }
}

void ASTUGameHUD::DrawCrossHair()
{
   const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f); 

   const float HalfLineSize = 10.0f;
   const float LineChickness = 2.0f;
   const FLinearColor LineColor = FLinearColor::Green;

   DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineChickness);
   DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineChickness);
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    //UE_LOG(LogTemp, Display, TEXT("Match state changed: %s"),*UEnum::GetValueAsString(State));

    if(IsValid(CurrentWidget))
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if(GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if(IsValid(CurrentWidget))
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
}
