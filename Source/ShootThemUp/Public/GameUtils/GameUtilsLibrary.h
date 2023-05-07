// Shoot Them Up Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameUtilsLibrary.generated.h"


DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAsyncSpawnActorDelegate, bool, bResult, FSoftObjectPath, LoadedReference, AActor*, SpawnedActor);
 
UCLASS()
class SHOOTTHEMUP_API UGameUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "CustomAssetLibrary", meta = (WorldContext = "WorldContextObject", DisplayName = "Spawn Actor (Async)"))
    static void AsyncSpawnActor(UObject* WorldContextObject, TSoftClassPtr<AActor> AssetPtr, FTransform SpawnTransform, const FAsyncSpawnActorDelegate& Callback);

protected:
    static void OnAsyncSpawnActorComplete(UObject* WorldContextObject, FSoftObjectPath Reference, FTransform SpawnTransform, FAsyncSpawnActorDelegate Callback);
};
