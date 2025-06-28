// Shoot Them Up Game. All rights reserved


#include "STUGameInstanceBase.h"
#include "AbilitySystemGlobals.h"

void USTUGameInstanceBase::Init()
{
    Super::Init();
    
    UAbilitySystemGlobals::Get().InitGlobalData();
}