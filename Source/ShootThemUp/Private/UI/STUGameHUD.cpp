// Shoot Them Up Game. All rights reserved


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD() 
{
    Super::DrawHUD();

    DrawCrossHair();
}

void ASTUGameHUD::DrawCrossHair() 
{
   const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f); // вычисляем центр экрана

   const float HalfLineSize = 10.0f;
   const float LineChickness = 2.0f;
   const FLinearColor LineColor = FLinearColor::Green;

   DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineChickness);
   DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineChickness);
}
