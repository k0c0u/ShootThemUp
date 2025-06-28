// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;
};
